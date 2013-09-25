/*  RetroArch - A frontend for libretro.
 *  Copyright (C) 2013 - Jason Fetters
 * 
 *  RetroArch is free software: you can redistribute it and/or modify it under the terms
 *  of the GNU General Public License as published by the Free Software Found-
 *  ation, either version 3 of the License, or (at your option) any later version.
 *
 *  RetroArch is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY;
 *  without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
 *  PURPOSE.  See the GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License along with RetroArch.
 *  If not, see <http://www.gnu.org/licenses/>.
 */

#include <dirent.h>
#include <sys/stat.h>

#import "apple/common/RetroArch_Apple.h"
#import "views.h"

#include "conf/config_file.h"
#include "file.h"

static const void* const associated_module_key = &associated_module_key;

@implementation RADirectoryItem
+ (RADirectoryItem*)directoryItemFromElement:(struct string_list_elem*)element
{
   RADirectoryItem* item = [RADirectoryItem new];
   item.path = @(element->data);
   item.isDirectory = element->attr.b;
   return item;
}
@end

enum file_action { FA_DELETE = 10000, FA_CREATE, FA_MOVE };
static void file_action(enum file_action action, NSString* source, NSString* target)
{
   NSError* error = nil;
   bool result = false;
   
   NSFileManager* manager = [NSFileManager defaultManager];
   
   switch (action)
   {
      case FA_DELETE: result = [manager removeItemAtPath:target error:&error]; break;
      case FA_CREATE: result = [manager createDirectoryAtPath:target withIntermediateDirectories:YES
                                        attributes:nil error:&error]; break;
      case FA_MOVE:   result = [manager moveItemAtPath:source toPath:target error:&error]; break;
   }

   if (!result && error)
      apple_display_alert(error.localizedDescription, @"Action failed");
}

@implementation RADirectoryList
{
   NSString* _path;
}

- (id)initWithPath:(NSString*)path delegate:(id<RADirectoryListDelegate>)delegate
{
   self = [super initWithStyle:UITableViewStylePlain];
   
   if (self)
   {
      _path = path;
      _directoryDelegate = delegate;

      self = [super initWithStyle:UITableViewStylePlain];
      self.title = path.lastPathComponent;
      self.hidesHeaders = YES;

      self.toolbarItems =
      @[
         [[UIBarButtonItem alloc] initWithBarButtonSystemItem:UIBarButtonSystemItemRefresh target:self
                                  action:@selector(refresh)],
         [[UIBarButtonItem alloc] initWithBarButtonSystemItem:UIBarButtonSystemItemFlexibleSpace target:self
                                  action:nil],
         [[UIBarButtonItem alloc] initWithBarButtonSystemItem:UIBarButtonSystemItemAdd target:self
                                  action:@selector(createNewFolder)]
      ];
      
      [self.tableView addGestureRecognizer:[[UILongPressGestureRecognizer alloc] initWithTarget:self
                      action:@selector(fileAction:)]];
   }

   return self;
}

- (void)viewWillAppear:(BOOL)animated
{
   [self refresh];
}
 
- (void)viewDidDisappear:(BOOL)animated
{
   [self reset];
}

- (NSArray*)sectionIndexTitlesForTableView:(UITableView*)tableView
{
   static NSArray* names = nil;

   if (!names)
      names = @[@"/", @"#", @"A", @"B", @"C", @"D", @"E", @"F", @"G", @"H", @"I", @"J", @"K", @"L",
                @"M", @"N", @"O", @"P", @"Q", @"R", @"S", @"T", @"U", @"V", @"W", @"X", @"Y", @"Z"];

   return names;
}

- (void)refresh
{
   // Need one array per section
   self.sections = [NSMutableArray array];
   
   for (NSString* i in [self sectionIndexTitlesForTableView:self.tableView])
      [self.sections addObject:[NSMutableArray arrayWithObject:i]];
   
   // List contents
   struct string_list* contents = dir_list_new(_path.UTF8String, 0, true);
   
   if (contents)
   {
      dir_list_sort(contents, true);
   
      for (int i = 0; i < contents->size; i ++)
      {
         const char* basename = path_basename(contents->elems[i].data);
      
         uint32_t section = isalpha(basename[0]) ? (toupper(basename[0]) - 'A') + 2 : 1;
         section = contents->elems[i].attr.b ? 0 : section;

         [self.sections[section] addObject:[RADirectoryItem directoryItemFromElement:&contents->elems[i]]];
      }
   
      dir_list_free(contents);
   }
   else
      apple_display_alert([NSString stringWithFormat:@"Browsed path is not a directory: %@", _path], 0);
   
   [self.tableView reloadData];
}

- (void)tableView:(UITableView*)tableView didSelectRowAtIndexPath:(NSIndexPath*)indexPath
{
   [self.directoryDelegate directoryList:self itemWasSelected:[self itemForIndexPath:indexPath]];
}

- (UITableViewCell*)tableView:(UITableView*)tableView cellForRowAtIndexPath:(NSIndexPath*)indexPath
{
   RADirectoryItem* path = [self itemForIndexPath:indexPath];
   uint32_t type_id = path.isDirectory ? 1 : 0;

   UITableViewCell* cell = nil;
   
   static NSString* const cell_types[2] = { @"file", @"folder" };
   if ([self getCellFor:cell_types[type_id] withStyle:UITableViewCellStyleDefault result:&cell])
   {
      static NSString* const icon_types[2] = { @"ic_file", @"ic_dir" };
      cell.imageView.image = [UIImage imageNamed:icon_types[type_id]];
   }

   cell.textLabel.text = [path.path lastPathComponent];
    
   return cell;
}


// File management
// Called as a selector from a toolbar button
- (void)createNewFolder
{
   UIAlertView* alertView = [[UIAlertView alloc] initWithTitle:@"Enter new folder name" message:@"" delegate:self
                                                  cancelButtonTitle:@"Cancel" otherButtonTitles:@"OK", nil];
   alertView.alertViewStyle = UIAlertViewStylePlainTextInput;
   alertView.tag = FA_CREATE;
   [alertView show];
}

// Called by the long press gesture recognizer
- (void)fileAction:(UILongPressGestureRecognizer*)gesture
{
   if (gesture.state == UIGestureRecognizerStateBegan)
   {
      CGPoint point = [gesture locationInView:self.tableView];
      NSIndexPath* indexPath = [self.tableView indexPathForRowAtPoint:point];
      
      if (indexPath)
      {
         self.selectedItem = [self itemForIndexPath:indexPath];

         UIActionSheet* menu = [[UIActionSheet alloc] initWithTitle:self.selectedItem.path.lastPathComponent delegate:self
                                                      cancelButtonTitle:@"Cancel" destructiveButtonTitle:nil
                                                      otherButtonTitles:@"Move", @"Rename", @"Delete", nil];
         menu.destructiveButtonIndex = 2;
         [menu showFromToolbar:self.navigationController.toolbar];
         
      }
   }
}

// Called by the action sheet created in (void)fileAction:
- (void)actionSheet:(UIActionSheet*)actionSheet clickedButtonAtIndex:(NSInteger)buttonIndex
{
   NSString* target = self.selectedItem.path;

   // Move
   if (buttonIndex == actionSheet.firstOtherButtonIndex)
      [self.navigationController pushViewController:[[RAFoldersList alloc] initWithFilePath:target] animated:YES];
   // Rename
   else if (buttonIndex == actionSheet.firstOtherButtonIndex + 1)
   {
      UIAlertView* alertView = [[UIAlertView alloc] initWithTitle:@"Enter new name" message:@"" delegate:self
                                                    cancelButtonTitle:@"Cancel" otherButtonTitles:@"OK", nil];
      alertView.alertViewStyle = UIAlertViewStylePlainTextInput;
      alertView.tag = FA_MOVE;
      [alertView textFieldAtIndex:0].text = target.lastPathComponent;
      [alertView show];
   }
   // Delete
   else if (buttonIndex == actionSheet.destructiveButtonIndex)
   {
      UIAlertView* alertView = [[UIAlertView alloc] initWithTitle:@"Really delete?" message:@"" delegate:self
                                                    cancelButtonTitle:@"Cancel" otherButtonTitles:@"OK", nil];
      alertView.tag = FA_DELETE;
      [alertView show];
   }
}

// Called by various alert views created in this class, the alertView.tag value is the action to take.
- (void)alertView:(UIAlertView*)alertView clickedButtonAtIndex:(NSInteger)buttonIndex
{
   if (buttonIndex != alertView.firstOtherButtonIndex)
      return;

   if (alertView.tag == FA_DELETE)
      file_action(FA_DELETE, nil, self.selectedItem.path);
   else
   {
      NSString* text = [alertView textFieldAtIndex:0].text;

      if (text.length)
         file_action(alertView.tag, self.selectedItem.path, [_path stringByAppendingPathComponent:text]);
   }

   [self refresh];
}


@end

@implementation RAModuleList
- (id)initWithGame:(NSString*)path delegate:(id<RAModuleListDelegate>)delegate
{
   self = [super initWithStyle:UITableViewStyleGrouped];
   
   if (self)
   {
      [self setTitle:path ? [path lastPathComponent] : @"Cores"];
      _moduleDelegate = delegate;

      // Load the modules with their data
      NSArray* moduleList = apple_get_modules();

      NSMutableArray* supported = [NSMutableArray arrayWithObject:@"Suggested Cores"];
      NSMutableArray* other = [NSMutableArray arrayWithObject:@"Other Cores"];
   
      for (RAModuleInfo* i in moduleList)
      {
         if (path && [i supportsFileAtPath:path]) [supported addObject:i];
         else                                     [other     addObject:i];
      }

      if (supported.count > 1)
         [self.sections addObject:supported];

      if (other.count > 1)
         [self.sections addObject:other];
   }

   return self;
}

- (void)tableView:(UITableView*)tableView didSelectRowAtIndexPath:(NSIndexPath*)indexPath
{
   [self.moduleDelegate moduleList:self itemWasSelected:[self itemForIndexPath:indexPath]];
}

- (void)infoButtonTapped:(id)sender
{
   RAModuleInfo* info = objc_getAssociatedObject(sender, associated_module_key);
   if (info && info.data)
      [self.navigationController pushViewController:[[RAModuleInfoList alloc] initWithModuleInfo:info] animated:YES];
   else
      apple_display_alert(@"No information available.", 0);
}

- (UITableViewCell*)tableView:(UITableView*)tableView cellForRowAtIndexPath:(NSIndexPath*)indexPath
{
   static NSString* const cell_id = @"module";

   UITableViewCell* cell = nil;
   if ([self getCellFor:cell_id withStyle:UITableViewCellStyleDefault result:&cell])
   {
      UIButton* infoButton = [UIButton buttonWithType:UIButtonTypeInfoDark];
      [infoButton addTarget:self action:@selector(infoButtonTapped:) forControlEvents:UIControlEventTouchUpInside];
      cell.accessoryView = infoButton;
   }
   
   RAModuleInfo* info = (RAModuleInfo*)[self itemForIndexPath:indexPath];
   cell.textLabel.text = info.description;
   objc_setAssociatedObject(cell.accessoryView, associated_module_key, info, OBJC_ASSOCIATION_RETAIN_NONATOMIC);

   return cell;
}
@end

@implementation RAFoldersList
{
   NSString* _path;
}

- (id)initWithFilePath:(NSString*)path
{
   self = [super initWithStyle:UITableViewStyleGrouped];
   
   if (self)
   {
      _path = path;

      // List contents
      struct string_list* contents = dir_list_new([_path stringByDeletingLastPathComponent].UTF8String, 0, true);
      NSMutableArray* items = [NSMutableArray arrayWithObject:@""];
      NSString* sourceDirectory = _path.stringByDeletingLastPathComponent;
   
      if (contents)
      {
         dir_list_sort(contents, true);

         for (int i = 0; i < contents->size; i ++)
         {
            if (contents->elems[i].attr.b)
            {
               const char* basename = path_basename(contents->elems[i].data);
               [items addObject:[sourceDirectory stringByAppendingPathComponent:@(basename)]];
            }
         }
   
         dir_list_free(contents);
      }

      [self setTitle:[@"Move " stringByAppendingString:_path.lastPathComponent]];
      [self.sections addObject:@[@"", [sourceDirectory stringByDeletingLastPathComponent]]];
      [self.sections addObject:items];
   }

   return self;
}

- (UITableViewCell*)tableView:(UITableView*)tableView cellForRowAtIndexPath:(NSIndexPath*)indexPath
{
   static NSString* const cell_id = @"Directory";
   
   UITableViewCell* cell = nil;
   [self getCellFor:cell_id withStyle:UITableViewCellStyleDefault result:&cell];

   cell.textLabel.text = [[self itemForIndexPath:indexPath] lastPathComponent];

   return cell;
}

- (void)tableView:(UITableView*)tableView didSelectRowAtIndexPath:(NSIndexPath*)indexPath
{
   NSString* targetPath = [[self itemForIndexPath:indexPath] stringByAppendingPathComponent:_path.lastPathComponent];
   file_action(FA_MOVE, _path, targetPath);
   [self.navigationController popViewControllerAnimated:YES];
}

@end

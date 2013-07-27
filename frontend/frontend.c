/* RetroArch - A frontend for libretro.
 * Copyright (C) 2010-2013 - Hans-Kristian Arntzen
 * Copyright (C) 2011-2013 - Daniel De Matteis
 *
 * RetroArch is free software: you can redistribute it and/or modify it under the terms
 * of the GNU General Public License as published by the Free Software Found-
 * ation, either version 3 of the License, or (at your option) any later version.
 *
 * RetroArch is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY;
 * without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
 * PURPOSE. See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along with RetroArch.
 * If not, see <http://www.gnu.org/licenses/>.
 */

#include "../general.h"
#include "../conf/config_file.h"
#include "../file.h"

#if defined(RARCH_CONSOLE)
#include "frontend_context.h"
frontend_ctx_driver_t *frontend_ctx;
#endif

#if defined(__QNX__)
#include <bps/bps.h>
#elif defined(__APPLE__)
#include <dispatch/dispatch.h>
#include <pthread.h>
#include "../apple/RetroArch/rarch_wrapper.h"
#endif

#if defined(HAVE_RGUI) || defined(HAVE_RMENU) || defined(HAVE_RMENU_XUI)
#define HAVE_MENU
#include "frontend/menu/menu_common.h"
#else
#undef HAVE_MENU
#endif

#include "../file_ext.h"

#ifdef RARCH_CONSOLE
#include "../config.def.h"

default_paths_t default_paths;

// Rename core filename executable to a more sane name.
static bool libretro_install_core(const char *path_prefix,
      const char *core_exe_path)
{
   char old_path[PATH_MAX], new_path[PATH_MAX];

   libretro_get_current_core_pathname(old_path, sizeof(old_path));

   strlcat(old_path, DEFAULT_EXE_EXT, sizeof(old_path));
   snprintf(new_path, sizeof(new_path), "%s%s", path_prefix, old_path);

   /* If core already exists, we are upgrading the core - 
    * delete existing file first. */
   if (path_file_exists(new_path))
   {
      RARCH_LOG("Removing temporary ROM file: %s.\n", new_path);
      if (remove(new_path) < 0)
         RARCH_ERR("Failed to remove file: %s.\n", new_path);
   }

   /* Now attempt the renaming of the core. */
   RARCH_LOG("Renaming core to: %s.\n", new_path);
   if (rename(core_exe_path, new_path) < 0)
   {
      RARCH_ERR("Failed to rename core.\n");
      return false;
   }

   strlcpy(g_settings.libretro, new_path,
         sizeof(g_settings.libretro));

   return true;
}

#define MAKE_DIR(x, name) { \
   RARCH_LOG("Checking directory name %s [%s]\n", name, x); \
   if (strlen(x) > 0) \
   { \
   if (!path_is_directory((x)) )\
   { \
      RARCH_WARN("Directory \"%s\" does not exists, creating\n", (x)); \
      if (mkdir((x), 0777) != 0) \
      { \
         RARCH_ERR("Could not create directory \"%s\"\n", (x)); \
      } \
   } \
   } \
}
#endif

static void system_preinit(void)
{
#if defined(__QNX__) && !defined(HAVE_BB10)
   //Initialize BPS libraries
   bps_initialize();
#elif defined(RARCH_CONSOLE)
   if (frontend_ctx->init)
      frontend_ctx->init();
#endif
}

static void rarch_get_environment(int argc, char *argv[])
{
#if defined(__QNX__) && !defined(HAVE_BB10)
   strlcpy(g_settings.libretro, "app/native/lib", sizeof(g_settings.libretro));
   strlcpy(g_extern.config_path, "app/native/retroarch.cfg", sizeof(g_extern.config_path));
   strlcpy(g_settings.video.shader_dir, "app/native/shaders_glsl", sizeof(g_settings.video.shader_dir));

   config_load();

   g_extern.verbose = true;
#elif defined(RARCH_CONSOLE)
#if defined(HAVE_LOGGER)
   g_extern.verbose = true;
   logger_init();
#elif defined(HAVE_FILE_LOGGER)
   g_extern.verbose = true;
   g_extern.log_file = fopen("/retroarch-log.txt", "w");
#endif
   g_extern.verbose = true;

   if (frontend_ctx->get_environment_settings)
      frontend_ctx->get_environment_settings(argc, argv);

   MAKE_DIR(default_paths.port_dir, "port_dir");
   MAKE_DIR(default_paths.system_dir, "system_dir");
   MAKE_DIR(default_paths.savestate_dir, "savestate_dir");
   MAKE_DIR(default_paths.sram_dir, "sram_dir");
   MAKE_DIR(default_paths.input_presets_dir, "input_presets_dir");

   config_load();

   init_libretro_sym(false);
   rarch_init_system_info();

   global_init_drivers();

#ifdef HAVE_LIBRETRO_MANAGEMENT
   char path_prefix[PATH_MAX];
#if defined(_WIN32)
   char slash = '\\';
#else
   char slash = '/';
#endif

   snprintf(path_prefix, sizeof(path_prefix), "%s%c", default_paths.core_dir, slash);

   char core_exe_path[256];
   snprintf(core_exe_path, sizeof(core_exe_path), "%sCORE%s", path_prefix, DEFAULT_EXE_EXT);

   // Save new libretro core path to config file and exit
   if (path_file_exists(core_exe_path))
      if (libretro_install_core(path_prefix, core_exe_path))
#ifdef _XBOX
    g_extern.system.shutdown = g_extern.system.shutdown;
#else
	 g_extern.system.shutdown = true;
#endif
#endif

#ifdef GEKKO
   /* Per-core input config loading */
   char core_name[64];

   libretro_get_current_core_pathname(core_name, sizeof(core_name));
   snprintf(g_extern.input_config_path, sizeof(g_extern.input_config_path), "%s/%s.cfg", default_paths.input_presets_dir, core_name);
   config_read_keybinds(g_extern.input_config_path);
#endif
#endif
}

static void system_shutdown(void)
{
#if defined(__QNX__)
   bps_shutdown();
#elif defined(__APPLE__)
   dispatch_async_f(dispatch_get_main_queue(), 0, apple_rarch_exited);
#endif
}

static int system_ctx_init(void)
{
#ifdef RARCH_CONSOLE
   if ((frontend_ctx = (frontend_ctx_driver_t*)frontend_ctx_init_first()) == NULL)
      return -1;
#endif

   return 0;
}

#if defined(__APPLE__)
static pthread_mutex_t apple_event_queue_lock = PTHREAD_MUTEX_INITIALIZER;

static struct
{
   void (*function)(void*);
   void* userdata;
} apple_event_queue[16];

static uint32_t apple_event_queue_size;

void apple_frontend_post_event(void (*fn)(void*), void* userdata)
{
   pthread_mutex_lock(&apple_event_queue_lock);

   if (apple_event_queue_size < 16)
   {
      apple_event_queue[apple_event_queue_size].function = fn;
      apple_event_queue[apple_event_queue_size].userdata = userdata;
      apple_event_queue_size ++;
   }

   pthread_mutex_unlock(&apple_event_queue_lock);
}

static void apple_free_main_wrap(struct rarch_main_wrap* wrap)
{
   if (wrap)
   {
      free((char*)wrap->libretro_path);
      free((char*)wrap->rom_path);
      free((char*)wrap->sram_path);
      free((char*)wrap->state_path);
      free((char*)wrap->config_path);
   }

   free(wrap);
}

static void process_events(void)
{
   pthread_mutex_lock(&apple_event_queue_lock);

   for (int i = 0; i < apple_event_queue_size; i ++)
      apple_event_queue[i].function(apple_event_queue[i].userdata);

   apple_event_queue_size = 0;

   pthread_mutex_unlock(&apple_event_queue_lock);
}

void* rarch_main(void* args)
#else
int rarch_main(int argc, char *argv[])
#endif
{
   if (system_ctx_init() != 0)
      return 0;

   system_preinit();

#if !defined(__APPLE__)
   rarch_main_clear_state();
   rarch_get_environment(argc, argv);
#endif


#if !defined(RARCH_CONSOLE)
#if defined(__APPLE__)
   struct rarch_main_wrap* argdata = (struct rarch_main_wrap*)args;
   int init_ret = rarch_main_init_wrap(argdata);
   apple_free_main_wrap(argdata);

   if (init_ret)
   {
      rarch_main_clear_state();
      dispatch_async_f(dispatch_get_main_queue(), (void*)1, apple_rarch_exited);
      return 0;
   }
#else
   rarch_init_msg_queue();
   int init_ret;
   if ((init_ret = rarch_main_init(argc, argv))) return init_ret;
#endif
#endif

#ifdef HAVE_MENU
#ifdef IOS
   char* system_directory = ios_get_rarch_system_directory();
   strlcpy(g_extern.savestate_dir, system_directory, sizeof(g_extern.savestate_dir));
   strlcpy(g_extern.savefile_dir, system_directory, sizeof(g_extern.savefile_dir));
   free(system_directory);
#endif

   menu_init();

#ifdef RARCH_CONSOLE
   if (frontend_ctx->process_args)
      frontend_ctx->process_args(argc, argv);
   g_extern.lifecycle_mode_state |= 1ULL << MODE_LOAD_GAME;
#else
   g_extern.lifecycle_mode_state |= 1ULL << MODE_GAME;
#endif

#ifndef RARCH_CONSOLE
   // If we started a ROM directly from command line,
   // push it to ROM history.
   if (!g_extern.libretro_dummy)
      menu_rom_history_push_current();
#endif

   for (;;)
   {
      if (g_extern.system.shutdown)
         break;
      else if (g_extern.lifecycle_mode_state & (1ULL << MODE_LOAD_GAME))
      {
         load_menu_game_prepare();

         // If ROM load fails, we exit RetroArch. On console it might make more sense to go back to menu though ...
         if (load_menu_game())
            g_extern.lifecycle_mode_state |= (1ULL << MODE_GAME);
         else
         {
#if defined(RARCH_CONSOLE) || defined(__QNX__)
            g_extern.lifecycle_mode_state |= (1ULL << MODE_MENU);
#else
#if defined(__APPLE__)
            // This needs to be here to tell the GUI thread that the emulator loop has stopped,
            // the (void*)1 makes it display the 'Failed to load game' message.
            dispatch_async_f(dispatch_get_main_queue(), (void*)1, apple_rarch_exited);
#endif
            return 1;
#endif
         }

         g_extern.lifecycle_mode_state &= ~(1ULL << MODE_LOAD_GAME);
      }
      else if (g_extern.lifecycle_mode_state & (1ULL << MODE_GAME))
      {
#ifdef RARCH_CONSOLE
         driver.input->poll(NULL);

         if (driver.video_poke->set_aspect_ratio)
            driver.video_poke->set_aspect_ratio(driver.video_data, g_settings.video.aspect_ratio_idx);
#endif

         while ((g_extern.is_paused && !g_extern.is_oneshot) ? rarch_main_idle_iterate() : rarch_main_iterate())
         {
#if defined(__APPLE__)
            process_events();
#endif

            if (!(g_extern.lifecycle_mode_state & (1ULL << MODE_GAME)))
               break;
         }
         g_extern.lifecycle_mode_state &= ~(1ULL << MODE_GAME);
      }
      else if (g_extern.lifecycle_mode_state & (1ULL << MODE_MENU))
      {
         g_extern.lifecycle_mode_state |= 1ULL << MODE_MENU_PREINIT;
         // Menu should always run with vsync on.
         video_set_nonblock_state_func(false);

         if (driver.audio_data)
            audio_stop_func();

         while (!g_extern.system.shutdown && menu_iterate())
         {
#if defined(__APPLE__)
            process_events();
#endif

            if (!(g_extern.lifecycle_mode_state & (1ULL << MODE_MENU)))
               break;
         }

         driver_set_nonblock_state(driver.nonblock_state);

         if (driver.audio_data && !audio_start_func())
         {
            RARCH_ERR("Failed to resume audio driver. Will continue without audio.\n");
            g_extern.audio_active = false;
         }

         g_extern.lifecycle_mode_state &= ~(1ULL << MODE_MENU);
      }
      else
         break;
   }

   g_extern.system.shutdown = false;
   menu_free();

   if (g_extern.config_save_on_exit && *g_extern.config_path)
      config_save_file(g_extern.config_path);

#ifdef GEKKO
   /* Per-core input config saving */
   config_save_keybinds(g_extern.input_config_path);
#endif

   if (g_extern.main_is_init)
      rarch_main_deinit();

#ifdef RARCH_CONSOLE
   global_uninit_drivers();
#endif
#else
   rarch_init_msg_queue();

   int init_ret;
   if ((init_ret = rarch_main_init(argc, argv))) return init_ret;

   while ((g_extern.is_paused && !g_extern.is_oneshot) ? rarch_main_idle_iterate() : rarch_main_iterate());
   rarch_main_deinit();
#endif

   rarch_deinit_msg_queue();

#ifdef PERF_TEST
   rarch_perf_log();
#endif

#ifdef RARCH_CONSOLE
#if defined(HAVE_LOGGER)
   logger_shutdown();
#elif defined(HAVE_FILE_LOGGER)
   if (g_extern.log_file)
      fclose(g_extern.log_file);
   g_extern.log_file = NULL;
#endif
   if (frontend_ctx->deinit)
      frontend_ctx->deinit();

   if (g_extern.lifecycle_mode_state & (1ULL << MODE_EXITSPAWN) && frontend_ctx->exitspawn)
      frontend_ctx->exitspawn();
#endif

   rarch_main_clear_state();

#ifdef __QNX__
   bps_shutdown();
#endif
   system_shutdown();

   return 0;
}

#ifndef __APPLE__
int main(int argc, char *argv[])
{
   return rarch_main(argc, argv);
}
#endif

#include <FL/Fl.H> // Has to be the first include!

#include "cfl.h"
#include "cfl_widget.h"

#include <FL/Enumerations.H>
#include <FL/Fl_Widget.H>
#include <FL/Fl_Window.H>
#include <FL/platform.H>
#include <stdarg.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#ifdef _WIN32
#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0501 /* need at least WinXP for this API, I think */
#endif
#include <windows.h>
#elif __APPLE__
#include <ApplicationServices/ApplicationServices.h>
#elif __ANDROID__
#include <queue>
#include <thread>
#else /* Assume X11 with XFT/fontconfig - this will break on systems using legacy Xlib fonts */
#include <fontconfig/fontconfig.h>
#define USE_XFT 1
#endif

#if defined(_MSC_VER) && _MSC_VER < 1900
#define snprintf _snprintf
#endif

int Fl_run(void) {
    return Fl::run();
}

int Fl_lock(void) {
    return Fl::lock();
}

void Fl_unlock(void) {
    Fl::unlock();
}

int Fl_awake_callback(Fl_Awake_Handler handler, void *data) {
    return Fl::awake(handler, data);
}

void Fl_awake(void) {
    Fl::awake();
}

void Fl_set_scrollbar_size(int v) {
    Fl::scrollbar_size(v);
}

int Fl_scrollbar_size(void) {
    return Fl::scrollbar_size();
}

void *Fl_grab(void) {
    return Fl::grab();
}

void Fl_set_grab(void *v) {
    Fl::grab((Fl_Window *)v);
}

int Fl_event(void) {
    return Fl::event();
}

int Fl_event_key(void) {
    return Fl::event_key();
}

int Fl_event_key_down(int key) {
    return Fl::event_key(key);
}

const char *Fl_event_text(void) {
    return Fl::event_text();
}

int Fl_event_button(void) {
    return Fl::event_button();
}

int Fl_event_clicks(void) {
    return Fl::event_clicks();
}

int Fl_event_x(void) {
    return Fl::event_x();
}

int Fl_event_y(void) {
    return Fl::event_y();
}

int Fl_event_x_root(void) {
    return Fl::event_x_root();
}

int Fl_event_y_root(void) {
    return Fl::event_y_root();
}
int Fl_event_dx(void) {
    return Fl::event_dx();
}

int Fl_event_dy(void) {
    return Fl::event_dy();
}

void Fl_get_mouse(int *x, int *y) {
    Fl::get_mouse(*x, *y);
}

int Fl_event_is_click(void) {
    return Fl::event_is_click();
}

int Fl_event_length(void) {
    return Fl::event_length();
}

int Fl_event_state(void) {
    return Fl::event_state();
}

int Fl_screen_x(void) {
    return Fl::x();
}

int Fl_screen_y(void) {
    return Fl::y();
}

int Fl_screen_h(void) {
    return Fl::h();
}

int Fl_screen_w(void) {
    return Fl::w();
}

void Fl_paste_text(Fl_Widget *widget, int src) {
    Fl::paste(*widget, src, Fl::clipboard_plain_text);
}

void Fl_paste_image(Fl_Widget *widget, int src) {
    Fl::paste(*widget, src, Fl::clipboard_image);
}

void Fl_set_scheme(const char *scheme) {
    Fl::scheme(scheme);
}

int Fl_scheme(void) {
    const char *v = Fl::scheme();
    if (!strcmp(v, "base")) {
        return 0;
    } else if (!strcmp(v, "gtk+")) {
        return 1;
    } else if (!strcmp(v, "gleam")) {
        return 2;
    } else {
        return 3;
    }
}

const char *Fl_scheme_string(void) {
    return Fl::scheme();
}

int Fl_visible_focus(void) {
    return Fl::visible_focus();
}

void Fl_set_visible_focus(int f) {
    Fl::visible_focus(f);
}

void Fl_set_box_type(int o, int n) {
    Fl::set_boxtype((Fl_Boxtype)o, (Fl_Boxtype)n);
}

unsigned int Fl_get_rgb_color(unsigned char r, unsigned char g, unsigned char b) {
    return fl_rgb_color(r, g, b);
}

void Fl_set_color(unsigned int c, unsigned char r, unsigned char g, unsigned char b) {
    Fl::set_color(c, r, g, b);
}

const char *Fl_get_font(int idx) {
    return Fl::get_font(idx);
}

unsigned char Fl_set_fonts(const char *c) {
    return Fl::set_fonts(c);
}

void Fl_set_font(int o, int n) {
    Fl::set_font(o, n);
}

void Fl_set_font2(int o, const char *n) {
    Fl::set_font(o, n);
}

void Fl_set_font_size(int sz) {
    Fl::lock();
    FL_NORMAL_SIZE = sz;
    Fl::unlock();
}

void Fl_add_handler(int (*ev_handler)(int ev)) {
    Fl::add_handler(ev_handler);
}

void Fl_open_display(void) {
    fl_open_display();
}

void Fl_close_display(void) {
    fl_close_display();
}

int Fl_box_dx(int boxtype) {
    return Fl::box_dx((Fl_Boxtype)boxtype);
}

int Fl_box_dy(int boxtype) {
    return Fl::box_dy((Fl_Boxtype)boxtype);
}

int Fl_box_dw(int boxtype) {
    return Fl::box_dw((Fl_Boxtype)boxtype);
}

int Fl_box_dh(int boxtype) {
    return Fl::box_dh((Fl_Boxtype)boxtype);
}

#ifdef __ANDROID__

class Buffer {
  private:
    using item = void *;
    std::queue<item> queue_;
    std::mutex m_;
    std::condition_variable cv_;

  public:
    void send(const item &i) {
        std::unique_lock<std::mutex> lock(m_);
        queue_.push(i);
        cv_.notify_one();
    }

    item recv(void) {
        if (!queue_.empty()) {
            std::unique_lock<std::mutex> lock(m_);
            cv_.wait(lock, [&](void) { return !queue_.empty(); });
            item result = queue_.front();
            queue_.pop();
            return result;
        } else {
            return nullptr;
        }
    }
} android_buffer;

#endif

void Fl_awake_msg(void *msg) {
    Fl_lock();
#ifndef __ANDROID__
    Fl::awake(msg);
#else
    android_buffer.send(msg);
#endif
    Fl_unlock();
}

void *Fl_thread_msg(void) {
#ifndef __ANDROID__
    return Fl::thread_message();
#else
    return android_buffer.recv();
#endif
}

int Fl_wait(void) {
    return Fl::wait();
}

double Fl_wait_for(double dur) {
    return Fl::wait(dur);
}

void Fl_add_timeout(double t, void (*timeout_h)(void *), void *data) {
    Fl::add_timeout(t, timeout_h, data);
}

void Fl_repeat_timeout(double t, void (*timeout_h)(void *), void *data) {
    Fl::repeat_timeout(t, timeout_h, data);
}

void Fl_remove_timeout(void (*timeout_h)(void *), void *data) {
    Fl::remove_timeout(timeout_h, data);
}

int Fl_has_timeout(void (*cb)(void *), void *arg) {
    return Fl::has_timeout(cb, arg);
}

int Fl_dnd(void) {
    return Fl::dnd();
}

void *Fl_first_window(void) {
    return (void *)Fl::first_window();
}

void *Fl_next_window(const void *prev) {
    return (void *)Fl::next_window((Fl_Window *)prev);
}

int Fl_should_program_quit(void) {
    return Fl::program_should_quit();
}

void Fl_program_should_quit(int flag) {
    Fl::program_should_quit(flag);
}

int Fl_event_inside(int x, int y, int w, int h) {
    return Fl::event_inside(x, y, w, h);
}

Fl_Widget *Fl_belowmouse(void) {
    return Fl::belowmouse();
}

void Fl_delete_widget(Fl_Widget *w) {
    Fl::delete_widget(w);
}

Fl_Widget_Tracker *Fl_Widget_Tracker_new(Fl_Widget *w) {
    return new Fl_Widget_Tracker(w);
}

int Fl_Widget_Tracker_deleted(Fl_Widget_Tracker *self) {
    return self->deleted();
}

void Fl_Widget_Tracker_delete(Fl_Widget_Tracker *self) {
    delete self;
}

void Fl_init_all(void) {
    fl_define_FL_ROUND_UP_BOX();
    fl_define_FL_SHADOW_BOX();
    fl_define_FL_ROUNDED_BOX();
    fl_define_FL_RFLAT_BOX();
    fl_define_FL_RSHADOW_BOX();
    fl_define_FL_DIAMOND_BOX();
    fl_define_FL_OVAL_BOX();
    fl_define_FL_PLASTIC_UP_BOX();
    fl_define_FL_GTK_UP_BOX();
    fl_define_FL_GLEAM_UP_BOX();
    fl_define_FL_SHADOW_LABEL();
    fl_define_FL_ENGRAVED_LABEL();
    fl_define_FL_EMBOSSED_LABEL();
    fl_define_FL_MULTI_LABEL();
    fl_define_FL_ICON_LABEL();
    fl_define_FL_IMAGE_LABEL();
    Fl::use_high_res_GL(1);
}

void Fl_redraw(void) {
    Fl::redraw();
}

int Fl_event_shift(void) {
    return Fl::event_shift();
}

int Fl_event_ctrl(void) {
    return Fl::event_ctrl();
}

int Fl_event_command(void) {
    return Fl::event_command();
}

int Fl_event_alt(void) {
    return Fl::event_alt();
}

void Fl_set_damage(int flag) {
    Fl::damage(flag);
}

int Fl_damage(void) {
    return Fl::damage();
}

int Fl_visual(int mode) {
    return Fl::visual(mode);
}

void Fl_own_colormap(void) {
    Fl::own_colormap();
}

Fl_Widget *Fl_pushed(void) {
    return Fl::pushed();
}

Fl_Widget *Fl_focus(void) {
    return Fl::focus();
}

void Fl_set_focus(void *wid) {
    Fl::focus((Fl_Widget *)wid);
}

double Fl_version(void) {
    return Fl::version();
}

int Fl_api_version(void) {
    return Fl::api_version();
}

int Fl_abi_version(void) {
    return Fl::abi_version();
}

void Fl_foreground(unsigned char r, unsigned char g, unsigned char b) {
    Fl::foreground(r, g, b);
}

void Fl_background(unsigned char r, unsigned char g, unsigned char b) {
    Fl::background(r, g, b);
}

void Fl_background2(unsigned char r, unsigned char g, unsigned char b) {
    Fl::background2(r, g, b);
}

void Fl_selection_color(unsigned char r, unsigned char g, unsigned char b) {
    Fl::set_color(FL_SELECTION_COLOR, r, g, b);
}

void Fl_inactive_color(unsigned char r, unsigned char g, unsigned char b) {
    Fl::set_color(FL_INACTIVE_COLOR, r, g, b);
}

void Fl_get_system_colors(void) {
    Fl::get_system_colors();
}

int Fl_handle(int ev, void *win) {
    int ret = 0;
    Fl::lock();
    ret = Fl::handle(ev, (Fl_Window *)win);
    Fl::unlock();
    Fl::awake();
    return ret;
}

int Fl_handle_(int ev, void *win) {
    int ret = 0;
    Fl::lock();
    ret = Fl::handle_(ev, (Fl_Window *)win);
    Fl::unlock();
    Fl::awake();
    return ret;
}

void Fl_add_idle(void (*cb)(void *), void *arg) {
    Fl::add_idle(cb, arg);
}

int Fl_has_idle(void (*cb)(void *), void *arg) {
    return Fl::has_idle(cb, arg);
}

void Fl_remove_idle(void (*cb)(void *), void *arg) {
    Fl::remove_idle(cb, arg);
}

void Fl_flush(void) {
    Fl::flush();
}

void Fl_set_screen_scale(int n, float val) {
    Fl::screen_scale(n, val);
}

float Fl_screen_scale(int n) {
    return Fl::screen_scale(n);
}

int Fl_screen_scaling_supported(void) {
    return Fl::screen_scaling_supported();
}

int Fl_screen_count(void) {
    return Fl::screen_count();
}

int Fl_screen_num(int x, int y) {
    return Fl::screen_num(x, y);
}

void Fl_screen_xywh(int *X, int *Y, int *W, int *H, int n) {
    Fl::screen_xywh(*X, *Y, *W, *H, n);
}

void Fl_screen_dpi(float *h, float *v, int n) {
    Fl::screen_dpi(*h, *v, n);
}

void Fl_screen_work_area(int *X, int *Y, int *W, int *H, int n) {
    Fl::screen_work_area(*X, *Y, *W, *H, n);
}

void *Fl_event_clipboard(void) {
    return Fl::event_clipboard();
}

const char *Fl_event_clipboard_type(void) {
    return Fl::event_clipboard_type();
}

int Fl_clipboard_contains(const char *type) {
    return Fl::clipboard_contains(type);
}

void Fl_event_dispatch(int (*cb)(int event, void *)) {
    Fl::event_dispatch((int (*)(int, Fl_Window *))cb);
}

int Fl_mac_os_version() {
#ifdef __APPLE__
    return fl_mac_os_version;
#endif
    return 0;
}

#ifdef _WIN32

#define i_load_private_font(PATH) AddFontResourceEx((PATH), FR_PRIVATE, 0)
#define v_unload_private_font(PATH) RemoveFontResourceEx((PATH), FR_PRIVATE, 0)

#elif __APPLE__
static int i_load_private_font(const char *pf) {
    int result = 0;
    CFErrorRef err;
    // Make a URL from the font name given
    CFURLRef fontURL = CFURLCreateFromFileSystemRepresentation(
        kCFAllocatorDefault, (const UInt8 *)pf, strlen(pf), false);
    // Try to load the font file
    if (CTFontManagerRegisterFontsForURL(fontURL, kCTFontManagerScopeProcess, &err)) {
        result = 1; // OK, we loaded the font, set this non-zero
    } else {
        printf("Failed loading font: %s\n", pf);
    }
    // discard the fontURL
    if (fontURL)
        CFRelease(fontURL);
    return result;
} // i_load_private_font

static void v_unload_private_font(const char *pf) {
    CFErrorRef err;
    // Make a URL from the font name given
    CFURLRef fontURL = CFURLCreateFromFileSystemRepresentation(
        kCFAllocatorDefault, (const UInt8 *)pf, strlen(pf), false);
    // Try to unregister the font
    CTFontManagerUnregisterFontsForURL(fontURL, kCTFontManagerScopeProcess, &err);
    if (fontURL)
        CFRelease(fontURL);
} // v_unload_private_font

#elif __ANDROID__

// Nothing!

#else /* Assume X11 with XFT/fontconfig - will break on systems using legacy Xlib fonts */

#define i_load_private_font(PATH) (int)FcConfigAppFontAddFile(NULL, (const FcChar8 *)(PATH))
#define v_unload_private_font(PATH) FcConfigAppFontClear(NULL)

#endif

#if !defined(__ANDROID__)
#define STB_TRUETYPE_IMPLEMENTATION
#include "stb_truetype.h"
#endif

const char *Fl_load_font(const char *path) {
#if !defined(__ANDROID__)
    stbtt_fontinfo font;
    FILE *fptr = fopen(path, "rb");
    if (!fptr)
        return nullptr;
    if (fseek(fptr, 0, SEEK_END)) {
        fclose(fptr);
        return nullptr;
    }
    long fsize = ftell(fptr);
    rewind(fptr);
    unsigned char *buffer = (unsigned char *)malloc(fsize);
    if (!buffer) {
        fclose(fptr);
        return nullptr;
    }
    size_t sz = fread(buffer, 1, fsize, fptr);
    fclose(fptr);
    if (sz != fsize) {
        free(buffer);
        return nullptr;
    }
    int init_ret = stbtt_InitFont(&font, buffer, stbtt_GetFontOffsetForIndex(buffer, 0));
    if (!init_ret) {
        free(buffer);
        return nullptr;
    }
    int length = 0;

    const char *info = stbtt_GetFontNameString(&font, &length, STBTT_PLATFORM_ID_MAC,
                                               STBTT_MAC_EID_ROMAN, STBTT_MAC_LANG_ENGLISH, 4);

    char *str = (char *)malloc(length + 1);
    snprintf(str, length + 1, "%s", info);
    auto ret = i_load_private_font(path);
    int f = 16;
    if (ret) {
        Fl::set_font(f, str);
    }
    free(buffer);
    return str;
#else
    return NULL;
#endif
}

void Fl_unload_font(const char *path) {
#if !defined(__ANDROID__)
    v_unload_private_font(path);
#endif
}

void Fl_add_system_handler(Fl_System_Handler handler, void *data) {
    Fl::add_system_handler(handler, data);
}
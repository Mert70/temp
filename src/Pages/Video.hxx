#ifndef _IHA_BASIC_PAGES_VIDEO_
#define _IHA_BASIC_PAGES_VIDEO_

#include <vector>
#include <Window.hxx>
#include <SDL3/SDL.h>
#include <stdexcept>

namespace basic
{
    class Video
    {

    public:
        Video() = default;
        Video(Window *_window, std::string _name = "video", int _width = 1280, int _height = 720)
        {
            if (!create(_window,_name,_width,_height))
            {
                throw std::runtime_error("Failed to create Video");
            }
        }
        ~Video() { destroy(); }

        bool create(Window *_window, std::string _name = "video", int _width = 1280, int _height = 720);
        void destroy();

        void setSize(int _width, int _height);
        void getSize(int &_width, int &_height) const;

        void setPosition(int _x, int _y);
        void getPosition(int &_x, int &_y) const;

        // RGBA8 buffer upload
        void updateFrame(const uint8_t *_data, int _size, int _pitch);
        void getFrameData(const uint8_t *&_data, int &_size, int &_pitch) const;

        // Rendering
        void draw();

        // --- Transformations ---
        void setRotation(float angleDeg); // döndürme
        float getRotation() const;

        void setFlip(bool horizontal, bool vertical); // yansıma
        void getFlip(bool &horizontal, bool &vertical) const;

        // --- Color adjustments ---
        void setBrightness(float value); // 0.0 - karanlık, 1.0 normal, >1 daha parlak
        float getBrightness() const;

        void setContrast(float value); // 1.0 normal, <1 daha düşük kontrast, >1 daha yüksek
        float getContrast() const;

        void setSaturation(float value); // 0 = grayscale, 1 = normal, >1 = fazla renk
        float getSaturation() const;

        void setGamma(float value); // tipik 0.8 - 2.2 arası
        float getGamma() const;

        void setAlpha(float value); // 0.0 saydam, 1.0 opak
        float getAlpha() const;

    private:
        bool m_created = false;
        bool m_updateSize = false;
        bool m_updatePosition = false;

        std::string m_name = "video";

        Window *m_window = nullptr;

        uint8_t *m_frame_data = nullptr;

        SDL_GPUTexture *m_gpu_texture = nullptr;

        SDL_Texture *m_texture = nullptr;
        SDL_Surface *m_surface = nullptr;

        unsigned int m_texture_id = 0;
        unsigned int m_pbo = 0;

        int m_video_size = 1280 * 720;
        int m_video_pitch = 1280;

        int m_width = 640;
        int m_height = 480;

        int m_pos_x = 0;
        int m_pos_y = 0;

        // state
        float m_rotation = 0.0f;
        bool m_flip_h = false;
        bool m_flip_v = false;

        float m_brightness = 1.0f;
        float m_contrast = 1.0f;
        float m_saturation = 1.0f;
        float m_gamma = 1.0f;
        float m_alpha = 1.0f;

        bool m_useOpenGL = false;
    };
}
// namespace basic

#endif

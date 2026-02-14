#include <Pages/Video.hxx>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <iostream>
#include <algorithm>
#include <Window.hxx>

#include <glad/glad.h>

namespace basic
{
    bool Video::create(Window *_window, std::string _name, int _width, int _height)
    {
        if (m_created)
            destroy();

        m_name = _name;
        if (_width <= 0)
            _width = 1280;
        if (_height <= 0)
            _height = 720;

        m_video_size = _width * _height;
        m_video_pitch = _width;

        m_window = _window;
        if (!m_window)
            return false;

        m_frame_data = new uint8_t[m_video_size * 4];
        if (!m_frame_data)
            return false;
        memset(m_frame_data, 255, m_video_size * 4);

#if USE_OPENGL
        m_useOpenGL = true;
        glGenTextures(1, &m_texture_id);
        glBindTexture(GL_TEXTURE_2D, m_texture_id);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_video_pitch, m_video_size / m_video_pitch, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glBindTexture(GL_TEXTURE_2D, 0);

        glGenBuffers(1, &m_pbo);
        glBindBuffer(GL_PIXEL_UNPACK_BUFFER, m_pbo);
        glBufferData(GL_PIXEL_UNPACK_BUFFER, m_video_size * 4, nullptr, GL_STREAM_DRAW);
        glBindBuffer(GL_PIXEL_UNPACK_BUFFER, 0);

#else
        m_texture = SDL_CreateTexture(m_window->getRenderer(), SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STREAMING, m_video_pitch, m_video_size / m_video_pitch);
        if (!m_texture)
        {
            destroy();
            return false;
        }

#endif
        m_created = true;
        return true;
    }
    void Video::destroy()
    {
        if (!m_created)
            return;
        if (!m_window)
            return;
        m_created = false;

        if (m_frame_data)
        {
            delete[] m_frame_data;
            m_frame_data = nullptr;
        }
        if (m_useOpenGL)
        {
            if (m_texture_id)
            {
                glDeleteTextures(1, &m_texture_id);
                glDeleteBuffers(1, &m_pbo);
            }
        }

        else
        {
            if (m_texture)
            {
                SDL_DestroyTexture(m_texture);
                m_texture = nullptr;
            }
        }

        m_window = nullptr;

        m_width = 640;
        m_height = 480;

        m_pos_x = 0;
        m_pos_y = 0;

        m_rotation = 0.0f;
        m_flip_h = false;
        m_flip_v = false;

        m_brightness = 1.0f;
        m_contrast = 1.0f;
        m_saturation = 1.0f;
        m_gamma = 1.0f;
        m_alpha = 1.0f;

        m_useOpenGL = false;
    }

    void Video::setSize(int _width, int _height)
    {
        m_width = _width;
        m_height = _height;
        m_updateSize = true;
    }
    void Video::getSize(int &_width, int &_height) const
    {
        _width = m_width;
        _height = m_height;
    }
    void Video::setPosition(int _x, int _y)
    {
        m_pos_x = _x;
        m_pos_y = _y;
        m_updatePosition = true;
    }
    void Video::getPosition(int &_x, int &_y) const
    {
        _x = m_pos_x;
        _y = m_pos_y;
    }
    void Video::updateFrame(const uint8_t *_data, int _size, int _pitch)
    {
        if (!m_created)
            return;

        if (_data && m_frame_data)
        {
            if (_size != m_video_size)
            {
                memset(m_frame_data, 255, m_video_size * 4);
            }
            if (_size == m_video_size && _pitch == m_video_pitch)
            {
                memcpy(m_frame_data, _data, _size * 4);
            }
            else
            {
                size_t index = 0;
                int copyWidth = std::min(m_video_pitch, _pitch);
                for (int i = 0; i < _size / _pitch; i++)
                {
                    index = i * m_video_pitch * 4;
                    if (index > m_video_size)
                        break;

                    memcpy(m_frame_data + index, _data, copyWidth * 4);
                }
            }
        }

        if (m_useOpenGL)
        {
            if (m_texture_id && m_pbo)
            {
                glBindBuffer(GL_PIXEL_UNPACK_BUFFER, m_pbo);
                
                void *ptr = glMapBufferRange(GL_PIXEL_UNPACK_BUFFER, 0, m_video_size * 4, GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT | GL_MAP_UNSYNCHRONIZED_BIT);
                if (ptr)
                {
                    memcpy(ptr, m_frame_data, m_video_size * 4);
                    glUnmapBuffer(GL_PIXEL_UNPACK_BUFFER);
                }

                glBindTexture(GL_TEXTURE_2D, m_texture_id);
                glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, m_video_pitch, m_video_size / m_video_pitch, GL_RGBA, GL_UNSIGNED_BYTE, 0);
                glBindTexture(GL_TEXTURE_2D, 0);
                glBindBuffer(GL_PIXEL_UNPACK_BUFFER, 0);
            }
        }
        else
        {
            if (m_texture && m_frame_data)
            {
                uint8_t *dst = nullptr;
                int pitch = 0;
                SDL_LockTexture(m_texture, nullptr, (void **)&dst, &pitch);
                memcpy(dst, _data, m_video_size * 4);
                SDL_UnlockTexture(m_texture);
            }
        }
    }
    void Video::getFrameData(const uint8_t *&_data, int &_size, int &_pitch) const
    {
        _data = m_frame_data;
        _size = m_video_size;
        _pitch = m_video_pitch;
    }
    void Video::draw()
    {

        if (m_updateSize)
        {
            ImGui::SetNextWindowSize(ImVec2((float)m_width, (float)m_height), ImGuiCond_Always);
            m_updateSize = false;
        }
        if (m_updatePosition)
        {
            ImGui::SetNextWindowPos(ImVec2((float)m_pos_x, (float)m_pos_y), ImGuiCond_Always);
            m_updatePosition = false;
        }
        ImGui::Begin(m_name.c_str());

        float winWidth = ImGui::GetWindowWidth() - 16;
        float winHeight = ImGui::GetWindowHeight() - 36;

        ImVec2 size(winWidth, winHeight);

        if (m_useOpenGL && m_texture_id)
        {

            ImGui::Image((ImTextureID)(intptr_t)m_texture_id, size);
        }
        else if (!m_useOpenGL && m_texture)
        {
            ImGui::Image((ImTextureID)m_texture, size);
        }
        else
        {
            ImGui::Text("No video frame available.");
        }

        ImGui::End();
    }
}
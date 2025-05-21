#include <algorithm>
#include <allegro5/allegro.h>
#include <string>

#include "Engine/Point.hpp"
#include "Slider.hpp"

Slider::Slider(float x, float y, float w, float h)
    : ImageButton("stage-select/slider.png", "stage-select/slider-blue.png", x, y),
      Bar("stage-select/bar.png", x, y, w, h),
      End1("stage-select/end.png", x, y + h / 2, 0, 0, 0.5, 0.5),
      End2("stage-select/end.png", x + w, y + h / 2, 0, 0, 0.5, 0.5) {
    Position.x += w;
    Position.y += h / 2;
    Anchor = Engine::Point(0.5, 0.5);
}
void Slider::Draw() const {
    // TODO HACKATHON-5 (3/4): The slider's component should be drawn here.
    this->Bar.Draw();
    this->End1.Draw();
    this->End2.Draw();
    // Draw the dragger.
    float mouse_x;
    al_draw_scaled_bitmap(bmp.get(), 0, 0, GetBitmapWidth(), GetBitmapHeight(),
                          Position.x - Anchor.x * GetBitmapWidth(), Position.y - Anchor.y * GetBitmapHeight(),
                          Size.x, Size.y, 0);   
}
void Slider::SetOnValueChangedCallback(std::function<void(float value)> onValueChangedCallback) {
    OnValueChangedCallback = onValueChangedCallback;
}
void Slider::SetValue(float value) {
    // TODO HACKATHON-5 (4/4): Set the value of the slider and call the callback.
    this->value = value;
    if (OnValueChangedCallback) {
        OnValueChangedCallback(value);
    }
}
void Slider::OnMouseDown(int button, int mx, int my) {
    if ((button & 1) && mouseIn)
        Down = true;
}
void Slider::OnMouseUp(int button, int mx, int my) {
    Down = false;
}
void Slider::OnMouseMove(int mx, int my) {
    ImageButton::OnMouseMove(mx, my);
    if (Down) {
        // [TODO 5 - 4] Attempt to update dragger position
        if (this->End1.Position.x <= mx && mx <= this->End2.Position.x) {
            this->Position.x = mx;
        }
        // Clamp
        float clamped = std::min(std::max(static_cast<float>(mx), Bar.Position.x), Bar.Position.x + Bar.Size.x);
        float value = (clamped - Bar.Position.x) / Bar.Size.x * 1.0f;
        SetValue(value);
    }
}

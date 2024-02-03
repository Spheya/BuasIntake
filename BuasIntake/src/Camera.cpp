#include "Camera.hpp"

Camera Camera::Main;

void Camera::moveTowards(float deltatime, tmpl8::vec2 target, tmpl8::vec2 speed) {
	tmpl8::vec2 t = tmpl8::vec2(std::exp(-deltatime * speed.x), std::exp(-deltatime * speed.y));
	position = position * t + target * (tmpl8::vec2(1.0f) - t);
}

tmpl8::vec2 Camera::toScreenSpace(tmpl8::vec2 position) {
	return (position - this->position) * scale + tmpl8::vec2(ScreenWidth / 2, ScreenHeight / 2);
}

tmpl8::vec2 Camera::scaleToScreenSpace(tmpl8::vec2 scale) {
	return scale * this->scale;
}

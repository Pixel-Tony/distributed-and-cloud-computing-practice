#include "Canvas.hpp"
#include "Image.hpp"

void Canvas::draw(sf::RenderTarget &target, sf::RenderStates states) const {
    target.draw(shape, states);
}

Canvas::Canvas(std::string &filepath) {
    loaded = false;
    {
        uint8_t *img_ptr = stbi_load(filepath.c_str(), &dimensions.x, &dimensions.y, &channels, STBI_rgb_alpha);
        if (img_ptr == nullptr) {
            return;
        } else {
            const int32_t imgSize{ dimensions.x * dimensions.y * bytesPerPixel };
            ptr.resize(imgSize);
            std::copy(img_ptr, img_ptr + imgSize, reinterpret_cast<uint8_t *>(ptr.data()));
        }
        stbi_image_free(img_ptr);
    }

    texture = std::make_shared<sf::Texture>();

    if (texture->create(dimensions.x, dimensions.y)) {
        texture->update(reinterpret_cast<uint8_t *>(this->ptr.data()));
        shape = sf::RectangleShape{ {static_cast<float>(dimensions.x), static_cast<float>(dimensions.y)} };
        shape.setTexture(texture.get());
        loaded = true;
    }
}

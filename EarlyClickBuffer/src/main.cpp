#include <Geode/Geode.hpp>
#include <Geode/modify/PlayerObject.hpp>
#include <algorithm>

using namespace geode::prelude;

// Cube-only jump buffer. A normal input is always forwarded to the game.
// When an airborne cube receives a fresh press, we also remember it briefly.
// A short tap is preserved even if the key is released before landing.
class $modify(BufferPlayer, PlayerObject) {
    struct Fields {
        int framesRemaining = 0;
        bool previousGrounded = false;
        bool injecting = false;
    };

    bool pushButton(PlayerButton button) {
        auto result = PlayerObject::pushButton(button);
        if (m_fields->injecting || !Mod::get()->getSettingValue<bool>("enabled"))
            return result;

        // The buffer is intended for the cube's primary jump only.
        // Don't alter ship, ball, UFO, wave, robot, spider or swing input.
        if (button == PlayerButton::Jump && m_isCube && !m_isOnGround) {
            auto configured = Mod::get()->getSettingValue<int64_t>("buffer-frames");
            m_fields->framesRemaining = std::clamp(static_cast<int>(configured), 1, 16);
        }
        return result;
    }

    void update(float dt) {
        PlayerObject::update(dt);
        auto& state = *m_fields;
        if (!Mod::get()->getSettingValue<bool>("enabled") || !m_isCube) {
            state.framesRemaining = 0;
            state.previousGrounded = m_isOnGround;
            return;
        }

        // On the first grounded update, retry the buffered press.
        // Release afterward so a tap doesn't become a permanently held key.
        if (state.framesRemaining > 0) {
            if (m_isOnGround && !state.previousGrounded) {
                state.injecting = true;
                PlayerObject::releaseButton(PlayerButton::Jump);
                PlayerObject::pushButton(PlayerButton::Jump);
                PlayerObject::releaseButton(PlayerButton::Jump);
                state.injecting = false;
                state.framesRemaining = 0;
            } else {
                --state.framesRemaining;
            }
        }
        state.previousGrounded = m_isOnGround;
    }
};

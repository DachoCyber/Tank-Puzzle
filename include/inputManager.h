#include <SFML/Graphics.hpp>

class InputManager {
private:

    sf::Keyboard::Key pressedKey; // initial state of input
    bool blocked = false;
public:

    InputManager ();
    ~InputManager ();

    void setPressedKey (sf::Keyboard::Key);
    sf::Keyboard::Key getPressedKey () const;

    bool getIsBlocked ();
    void blockInput ();
    void unblockInput ();
};
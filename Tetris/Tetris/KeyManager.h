#pragma once

#include <vector>

#include "eKeyState.h"
#include "eKey.h"

struct KeyInfo
{
    eKeyState State;
    bool bPrevPressed;
};

class KeyManager
{
public:
    static void CreateInstance();
    static void DeleteInstance();
    static KeyManager* GetInstance();

    void Update();
    eKeyState GetKeyState(eKey key) const;

private:
    KeyManager();
    ~KeyManager() = default;

    inline bool hasPressedKey(int key);

private:
    static KeyManager* mKeyManager;
    static const int KEYS[static_cast<int>(eKey::Last)];

    enum
    {
        KEY_PRESSED_MASK = 0x8000
    };

    std::vector<KeyInfo> mKeyInfos;
};

inline bool KeyManager::hasPressedKey(int key)
{
    return (GetAsyncKeyState(key) & KEY_PRESSED_MASK) > 0 ? true : false;
}

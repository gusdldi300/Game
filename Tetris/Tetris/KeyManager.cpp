
#include <cassert>
#include <wtypes.h>

#include "Engine.h"
#include "KeyManager.h"

KeyManager* KeyManager::mKeyManager = nullptr;
const int KeyManager::KEYS[static_cast<int>(eKey::Last)] =
{
    VK_UP,
    VK_DOWN,
    VK_LEFT,
    VK_RIGHT,

    'W',
    'S',
    'A',
    'D',

    VK_RETURN,
    VK_SPACE,
    VK_ESCAPE
};

void KeyManager::CreateInstance()
{
    assert(mKeyManager == nullptr);

    mKeyManager = new KeyManager();
}

void KeyManager::DeleteInstance()
{
    assert(mKeyManager == nullptr);
    
    delete mKeyManager;
}

KeyManager* KeyManager::GetInstance()
{
    assert(mKeyManager != nullptr);

    return mKeyManager;
}

KeyManager::KeyManager()
{
    for (unsigned int i = 0; i < static_cast<unsigned int>(eKey::Last); ++i)
    {
        KeyInfo keyInfo = { eKeyState::None, false };

        mKeyInfos.push_back(keyInfo);
    }
}

eKeyState KeyManager::GetKeyState(eKey key) const
{
    return mKeyInfos[static_cast<int>(key)].State;
}

void KeyManager::Update()
{
    HWND hFocusedWindow = GetFocus();
    if (hFocusedWindow != Engine::GetInstance()->GetWindowHandle())
    {
        for (unsigned int i = 0; i < static_cast<unsigned int>(eKey::Last); ++i)
        {
            mKeyInfos[i].State = eKeyState::None;
            mKeyInfos[i].bPrevPressed = false;
        }

        return;
    }

    for (unsigned int i = 0; i < static_cast<unsigned int>(eKey::Last); ++i)
    {
        if (hasPressedKey(KEYS[i]))
        {
            if (mKeyInfos[i].bPrevPressed)
            {
                mKeyInfos[i].State = eKeyState::Hold;
            }
            else
            {
                mKeyInfos[i].State = eKeyState::Press;
            }

            mKeyInfos[i].bPrevPressed = true;
        }
        else
        {
            if (mKeyInfos[i].bPrevPressed)
            {
                mKeyInfos[i].State = eKeyState::Release;
            }
            else
            {
                mKeyInfos[i].State = eKeyState::None;
            }

            mKeyInfos[i].bPrevPressed = false;
        }
    }
}


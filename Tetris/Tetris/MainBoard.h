#pragma once

#include <wtypes.h>
#include <list>

#include "GameStage.h"
#include "Tetromino.h"

// Todo: #include, 전방선언 정리하기
class Position;
class TetrominoManager;
class HoldManager;

class MainBoard
{
public:
    MainBoard(Tetromino* tetromino);
    virtual ~MainBoard();

    bool MoveTetrominoOneStep(eDirection direction);
    bool RotateTetrominoCW();

    bool AddHold(TetrominoManager* tetrominoManager);
    bool UseHold(TetrominoManager* tetrominoManager);

    bool RiseBlocksOneStep();
    unsigned int ClearFullLines();
    void LockDownTetromino(TetrominoManager* tetrominoManager);
    bool IsGameOver() const;

    void Reset();

    const eColor* const* GetColorBoard() const;

    const Tetromino* GetActiveTetromino() const;
    const std::vector<Position> GetGhostTetrominoBlockPositions() const;

    void ReleaseAndSetNextActiveTetrominoFrom(TetrominoManager* tetrominoManager);

private:
    bool canPlaceBlocksOnBoard(const std::vector<Position>& blockPositions) const;
    void respawnActiveTetromino();
    //void moveBlocksOneStep(std::vector<Position>* outBlockPositions, eDirection direction);

public:
    enum
    {
        SPAWN_ZONE_ROW_SIZE = 2U,
        BOARD_ROW_SIZE = 20U + SPAWN_ZONE_ROW_SIZE,
        BOARD_COL_SIZE = 10U
    };

private:
    enum
    {
        SPAWN_TETROMINO_ROW = 0U,
        SPAWN_TETROMINO_COL = 3U
    };

    Tetromino* mActiveTetromino;

    eColor** mColorBoard;
};



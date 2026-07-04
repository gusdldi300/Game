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

    // Todo: Move to private
    bool MoveTetrominoOneStep(eDirection direction);
    bool RotateTetrominoCW();

    bool AddHold(TetrominoManager* tetrominoManager);
    bool UseHold(TetrominoManager* tetrominoManager);

    bool RiseBlocksOneStep();
    unsigned int ClearFullLines();
    void LockDownTetromino(TetrominoManager* tetrominoManager);
    bool IsGameOver() const;

    const bool* const* GetGrid() const;

    // Todo: Check later
    const Tetromino* GetActiveTetromino() const;
    const Tetromino* GetHoldTetrominoOrNull() const;
    const std::vector<Position> GetGhostTetrominoBlockPositions() const;


    // Todo: 사용하기 헷갈림
    void ReleaseActiveTetromino(TetrominoManager* tetrominoManager);
    void SetNextTetrominoFrom(TetrominoManager* tetrominoManager);

private:
    bool canPlaceBlocksOnBoard(const std::vector<Position>& blockPositions) const;
    void respawnActiveTetromino();
    //void moveBlocksOneStep(std::vector<Position>* outBlockPositions, eDirection direction);

public:
    // Todo: 쓸모없는 정적 변수 너무 많음, cpp 에 초기화하기
    static const unsigned int SPAWN_ZONE_ROW_SIZE = 2U;

    static const unsigned int BOARD_ROW_SIZE = 20U + SPAWN_ZONE_ROW_SIZE;
    static const unsigned int BOARD_COL_SIZE = 10U;

private:
    static const unsigned int SPAWN_TETROMINO_ROW = 0U;
    static const unsigned int SPAWN_TETROMINO_COL = 3U;

    Tetromino* mActiveTetromino;
    Tetromino* mHoldTetrominoOrNull;
    bool mbHoldUsed;

    bool** mbBoard;
};



#pragma once

class AElevatorManager;

class ElevatorManagable
{
public:
    void SetManager(AElevatorManager*);

protected:
    AElevatorManager* Manager = nullptr;
    int IdxInManager;

    virtual int GetIdxInManager() = 0;
    virtual void BeginPlay();
};


#pragma once


class prLayer;


class prLayerManager
{
public:
    prLayerManager();
    ~prLayerManager();


private:
    prLayer  **mpLayers;
};
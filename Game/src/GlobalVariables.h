#pragma once

#include "GameState/IGameState.h"
#include "GameObject/GameObject.h"

class GlobalVariables{

    public:
        //Constructor
        GlobalVariables(){};

        //Destructor
        ~GlobalVariables() {};

        //static reference getter
        static GlobalVariables& getInstance(){
            static GlobalVariables instance;
            return instance;
        }

        //==========================================
        // GETTERS AND SETTERS
        //==========================================
        void    setGameState(IGameState::stateType t)       {     game = t;           };
        IGameState::stateType getGameState()                {     return game;        };
        void    setPlayer(GameObject* p)                    {     player = p;         };
        GameObject* getPlayer()                             {     return player;      };
        void    setServer(bool s)                           {     server = s;         };
        bool    getServer()                                 {     return server;      };
        void    setOnline(bool o)                           {     online = o;         };
        bool    getOnline()                                 {     return online;      };
        void    setDistanceLoD(float d)                     {     distanceLoD = d;    };
        float   getDistanceLoD()                            {     return distanceLoD; };
        void    setLanguage(int l)                          {     language = l;       };
        int     getLanguage()                               {     return language;    };
        void    setMaxDTime(float d)                        {     maxDTime = d;       };
        float   getMaxDTime()                               {     return maxDTime;    };
        void    setVolume(float v)                          {     volume = v;         };
        float   getVolume()                                 {     return volume;      };
        void    setCountdown(int c)                         {     countdown = c;      };
        int     getCountdown()                              {     return countdown;   };
        void    setIgnoreInput(bool i)                      {     ignoreInput = i;    };
        bool    getIgnoreInput()                            {     return ignoreInput; };
        void    setGameLoaded(bool g)                       {     gameLoaded = g;     };
        bool    getGameLoaded()                             {     return gameLoaded;  };
        void    setSelectedPlayer(int s)                    {     selectedPlayer = s; };
        int     getSelectedPlayer()                         {     return selectedPlayer;};
        void    setFixedPlayer(bool f)                      {     fixedPlayer = f;    };
        bool    getFixedPlayer()                            {     return fixedPlayer; };
        void    setSelecting(bool s)                        {     selecting = s;      };
        bool    getSelecting()                              {     return selecting;   };
        void    setFullscreen(bool f)                       {     fullscreen = f;     };
        bool    getFullscreen()                             {     return fullscreen;  };
        
    private:

        //==========================================
        // GLOBAL VARIABLES
        //==========================================
        IGameState::stateType game; 
        GameObject* player; 
        bool server;
        bool online;
        //Max value for dTime
        float maxDTime = 1.0f/30.0f;

        //Countdown value
        int countdown;
        bool gameLoaded = false;

        //Variable for options
        float volume = 1.0;
        int language;
        bool ignoreInput = false;
        bool fullscreen = false;

        //Player selection
        int selectedPlayer = 0;     //Current player to choose
        bool fixedPlayer = false;   //If we chose a player
        bool selecting = false;     //If we are chosing a different player

        //==========================================
        // AI DATA
        //==========================================
        float distanceLoD;      //Define value in RenderManager init().     Value of distance player and other object or player to calculate LoD
};

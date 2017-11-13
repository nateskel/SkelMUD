cmake_minimum_required(VERSION 3.1)
project(SkelMUD)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES
        SkelMUD/Areas/Area.h
        SkelMUD/Areas/Area.cpp
        SkelMUD/Areas/AreaManager.h
        SkelMUD/Areas/AreaManager.cpp
        SkelMUD/Connection.cpp
        SkelMUD/Connection.h
        SkelMUD/Entity.cpp
        SkelMUD/Entity.h
        SkelMUD/File.cpp
        SkelMUD/File.h
        SkelMUD/Game.cpp
        SkelMUD/Game.h
        SkelMUD/Logger.cpp
        SkelMUD/Logger.h
        SkelMUD/Network.cpp
        SkelMUD/Network.h
        SkelMUD/Planets/Planet.cpp
        SkelMUD/Planets/Planet.h
        SkelMUD/Player.cpp
        SkelMUD/Player.h
        SkelMUD/Sender.cpp
        SkelMUD/Sender.h
        SkelMUD/SkelMUD.cpp
        SkelMUD/TextProcessor.cpp
        SkelMUD/TextProcessor.h
        SkelMUD/Tokenizer.cpp
        SkelMUD/Tokenizer.h
        SkelMUD/Utils.cpp
        SkelMUD/Utils.h
        SkelMUD/States/GameState.cpp
        SkelMUD/States/GameState.h
        SkelMUD/GameData.cpp
        SkelMUD/GameData.h
        SkelMUD/States/TestState.cpp
        SkelMUD/States/TestState.h
        SkelMUD/States/PlayingState.cpp
        SkelMUD/States/PlayingState.h
        SkelMUD/Skexml/Node.cpp
        SkelMUD/Skexml/Node.h
        SkelMUD/Skexml/SkexmlParser.cpp
        SkelMUD/Skexml/SkexmlParser.h
        SkelMUD/Test/SkexmlTest.cpp
        SkelMUD/Test/SkexmlTest.h
        SkelMUD/States/LoginState.cpp
        SkelMUD/States/LoginState.h
        SkelMUD/Accounts/Accounts.cpp
        SkelMUD/Accounts/Accounts.h
        SkelMUD/Accounts/Account.cpp
        SkelMUD/Accounts/Account.h
        SkelMUD/Format.cpp
        SkelMUD/Format.h
        SkelMUD/States/CreateCharacterState.cpp
        SkelMUD/States/CreateCharacterState.h
        SkelMUD/Players.cpp
        SkelMUD/Players.h
        SkelMUD/Skills/Skill.cpp
        SkelMUD/Skills/Skill.h
        SkelMUD/Skills/Skills.cpp
        SkelMUD/Skills/Skills.h
        SkelMUD/Races/Race.cpp
        SkelMUD/Races/Race.h
        SkelMUD/Races/Races.cpp
        SkelMUD/Races/Races.h SkelMUD/Classes/CharacterClass.cpp SkelMUD/Classes/CharacterClass.h SkelMUD/Classes/CharacterClasses.cpp SkelMUD/Classes/CharacterClasses.h SkelMUD/Characters/Characters.cpp SkelMUD/Characters/Characters.h SkelMUD/Planets/Planets.cpp SkelMUD/Planets/Planets.h SkelMUD/States/BuildingState.cpp SkelMUD/States/BuildingState.h SkelMUD/Config/Configuration.cpp SkelMUD/Config/Configuration.h SkelMUD/Ships/Ship.cpp SkelMUD/Ships/Ship.h SkelMUD/Areas/Area.cpp SkelMUD/Areas/Area.h SkelMUD/Areas/AreaManager.cpp SkelMUD/Areas/AreaManager.h SkelMUD/Areas/Room.cpp SkelMUD/Areas/Room.h SkelMUD/Ships/Ships.cpp SkelMUD/Ships/Ships.h SkelMUD/States/StateFactory.cpp SkelMUD/States/StateFactory.h)

add_executable(SkelMUD ${SOURCE_FILES} SkelMUD/SkelMUD.cpp)
FIND_PACKAGE(Threads)
# FIND_PACKAGE(Threads) sets the following:
# CMAKE_THREAD_LIBS_INIT        - the thread library to link into an application (-lpthread )
# CMAKE_USE_SPROC_INIT          - are we using sproc?
# CMAKE_USE_WIN32_THREADS_INIT  - are we using WIN32 threads
# CMAKE_USE_PTHREADS_INIT       - are we using pthreads
# CMAKE_HP_PTHREADS_INIT        - are we using hp pthreads

TARGET_LINK_LIBRARIES(SkelMUD ${CMAKE_THREAD_LIBS_INIT})
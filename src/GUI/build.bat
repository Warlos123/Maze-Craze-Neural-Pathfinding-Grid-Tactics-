
@echo off
echo Compilando Maze Craze...
 
g++ -std=c++17 window.cpp PlayerVSIA/PlayerVSIA.cpp Multiplayer/Multiplayer.cpp GameScreen/GameScreen.cpp VictoryScreen/VictoryScreen.cpp ../Code/Game/Game.cpp ../Code/Graph/Graph.cpp ../Code/MazeGenerator/MazeGenerator.cpp ../Code/Path/Path.cpp ../Code/Player/Player.cpp -o app -lsfml-graphics -lsfml-window -lsfml-system
 
if %errorlevel% neq 0 (
    echo Error en la compilacion.
    pause
    exit /b %errorlevel%
)
 
echo Compilacion exitosa. Ejecutando...
app.exe
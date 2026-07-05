#include <SFML/Graphics.hpp>
#include <iostream>
#include <random>
#include <optional>
#include <string>
enum class GameState
    {
        Menu,
        Serve,
        Playing,
        Paused,
        GameOver
    };
enum class GameMode
    {
        OnePlayer,
        TwoPlayer
    };
GameState currentState = GameState::Menu;
GameMode currentMode = GameMode::TwoPlayer;
void centerline(sf::RectangleShape &dash,const float &Windowheight, sf::RenderWindow &window,const float &WindowWidth)
{
    for(float i = dash.getSize().y/2; i<=Windowheight;i += 1.5*dash.getSize().y)
    {
        dash.setPosition({WindowWidth/2.f,i});
        window.draw(dash);
    }
}
void serveBall(sf::RectangleShape &left_paddle, sf::RectangleShape &right_paddle, sf::CircleShape &ball, float &ballVelocityX, float& ballVelocityY,const sf::Vector2f& Center, float &BallSpeed, std::mt19937& engine, std::uniform_int_distribution<int>& flip,std::uniform_real_distribution<float>&leftserve,std::uniform_real_distribution<float>&rightserve,float InitialBallspeed)
{
    ball.setPosition(Center);
    const float PI = 3.14159265358979323846f;
    int Direction = flip(engine);
    float theta;
    if(Direction == 0)
    {
        theta = rightserve(engine);
    }
    else
    {
        theta = leftserve(engine);
    }   
    theta *= PI/180.f;
    BallSpeed = InitialBallspeed;
    ballVelocityX = BallSpeed*std::cos(theta);
    ballVelocityY = BallSpeed*std::sin(theta); 
    left_paddle.setPosition({25.f,300.f}); 
    right_paddle.setPosition({775.f,300.f});
}

int main()
{
    sf::RenderWindow window(sf::VideoMode({800,600}), "Pong");
    window.setVerticalSyncEnabled(true);

    sf::RectangleShape left_paddle;
    left_paddle.setSize({10.f,100.f});
    left_paddle.setOrigin({5.f,50.f});
    left_paddle.setPosition({25.f,300.f});
    left_paddle.setFillColor(sf::Color::Black);

    sf::RectangleShape right_paddle;
    right_paddle.setSize({10.f,100.f});
    right_paddle.setOrigin({5.f,50.f});
    right_paddle.setPosition({775.f,300.f});
    right_paddle.setFillColor(sf::Color::Black);

    sf::RectangleShape dash;

    sf::RectangleShape PausedOverlay;

    sf::CircleShape ball;
    ball.setFillColor(sf::Color::White);
    ball.setRadius(7.f);
    ball.setOrigin({7.f,7.f});
    ball.setPosition({400.f,300.f});

    const float PI = 3.14159265358979323846f;
    const float BallRadius = ball.getRadius();
    const float PaddleHalfWidth = (left_paddle.getSize().x)/2.f;
    const float PaddleHalfHeight = (left_paddle.getSize().y)/2.f;
    const float WindowHeight = static_cast<float>(window.getSize().y);
    const float WindowWidth = static_cast<float>(window.getSize().x);
    const sf::Vector2f Center = {WindowWidth/2.f,WindowHeight/2.f};
    float paddleVelocityX=0.f;
    float paddleVelocityY= 300.f;
    const float InitialBallspeed = 300.f;
    float BallSpeed = 320.f;
    const float maxBallspeed = 600.f;
    float ballVelocityX;
    float ballVelocityY;
    int leftScore = 0;
    int rightScore = 0;
    std::random_device rd;
    std::mt19937 engine(rd());
    std::uniform_int_distribution<int> flip(0,1);
    std::uniform_real_distribution<float> rightserve(-60.f,60.f);
    std::uniform_real_distribution<float> leftserve(120.f,240.f);
    sf::Font font;
    if(!font.openFromFile("assets/Font/PixeloidSans-Bold.ttf"))
    {
        std::cerr << "Failed to load font\n";
        return -1;
    }
    sf::Text scoreText(font);
    scoreText.setFillColor(sf::Color::White);
    scoreText.setCharacterSize(45);
    sf::Text onePlayerText(font);
    sf::Text twoPlayerText(font);
    sf::Text exitText(font);
    sf::Text Pong(font);
    sf::Text Astrath(font);
    Pong.setCharacterSize(80);
    Astrath.setCharacterSize(Pong.getCharacterSize()/4);
    onePlayerText.setCharacterSize(Pong.getCharacterSize()*2/3);
    twoPlayerText.setCharacterSize(Pong.getCharacterSize()*2/3);
    exitText.setCharacterSize(Pong.getCharacterSize()*2/3);
    Astrath.setString("By Astrath");
    Pong.setString("Ocean Pong");
    onePlayerText.setString("One Player");
    twoPlayerText.setString("Two Player");
    exitText.setString("Exit");
    Astrath.setFillColor(sf::Color(255,16,240));
    Pong.setOutlineThickness(7);
    Pong.setOutlineColor(sf::Color::Blue);
    Pong.setPosition({(WindowWidth-Pong.getLocalBounds().size.x)/2,WindowHeight/14});
    Astrath.setPosition({(WindowWidth-Astrath.getLocalBounds().size.x)/2,WindowHeight*4/14});
    onePlayerText.setPosition({(WindowWidth-onePlayerText.getLocalBounds().size.x)/2.f,WindowHeight*6/14});
    twoPlayerText.setPosition({(WindowWidth-twoPlayerText.getLocalBounds().size.x)/2.f,WindowHeight*9/14});
    exitText.setPosition({(WindowWidth-exitText.getLocalBounds().size.x)/2.f,WindowHeight*12/14});
    sf::Text WinnerText(font);
    sf::Text PlayAgain(font);
    sf::Text MenuText(font);
    PlayAgain.setString("Play Again");
    MenuText.setString("Menu");
    WinnerText.setFillColor(sf::Color(0,255,255));
    WinnerText.setCharacterSize(Pong.getCharacterSize()/1.5);
    PlayAgain.setCharacterSize(Pong.getCharacterSize()*2/3);
    MenuText.setCharacterSize(Pong.getCharacterSize()*2/3);
    PlayAgain.setPosition({(WindowWidth-PlayAgain.getLocalBounds().size.x)/2,WindowHeight*6/14});
    MenuText.setPosition({(WindowWidth-MenuText.getLocalBounds().size.x)/2,WindowHeight*9/14});
    sf::Text PausedText(font);
    sf::Text Resume(font);
    sf::Text Restart(font);
    PausedText.setCharacterSize(Pong.getCharacterSize()*2/3);
    Resume.setCharacterSize(Pong.getCharacterSize()*2/3);
    Restart.setCharacterSize(Pong.getCharacterSize()*2/3);
     PausedText.setString("Paused");
    Resume.setString("Resume");
    Restart.setString("Restart");
    PausedText.setFillColor(sf::Color(0,255,255));
    PausedText.setPosition({(WindowWidth-PausedText.getLocalBounds().size.x)/2,WindowHeight*1/14});
    Resume.setPosition({(WindowWidth-Resume.getLocalBounds().size.x)/2,WindowHeight*3/11.f});
    Restart.setPosition({(WindowWidth-Restart.getLocalBounds().size.x)/2,WindowHeight*6/13}); 
    dash.setSize({PaddleHalfWidth,PaddleHalfHeight/2});
    dash.setOrigin({PaddleHalfWidth/2,PaddleHalfHeight/4});
    dash.setFillColor(sf::Color(220,220,220,80));
    PausedOverlay.setSize({WindowWidth,WindowHeight});
    PausedOverlay.setFillColor(sf::Color(0,80,80,80));

    sf::Clock clock;
    sf::Clock rest;
    //Game loop
    while(window.isOpen())
    {
        auto mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
        while(const std::optional event = window.pollEvent())
        {
            if(event->is<sf::Event::Closed>())
            {
               window.close();
            }
            if(const auto* mousePressed = event->getIf<sf::Event::MouseButtonPressed>())
            {
                if(mousePressed->button==sf::Mouse::Button::Left)
                {
                   switch(currentState)
                   {
                    case GameState::Menu:
                    {
                        if(onePlayerText.getGlobalBounds().contains(mousePos))
                        {
                            serveBall(left_paddle,right_paddle,ball,ballVelocityX, ballVelocityY, Center,BallSpeed,engine,flip,leftserve,rightserve,InitialBallspeed);
                            rest.restart();
                            currentMode = GameMode::OnePlayer;
                            currentState = GameState::Serve;
                        }
                        else if(twoPlayerText.getGlobalBounds().contains(mousePos))
                        {
                            serveBall(left_paddle,right_paddle,ball,ballVelocityX, ballVelocityY, Center,BallSpeed,engine,flip,leftserve,rightserve,InitialBallspeed);
                            rest.restart();
                            currentMode = GameMode::TwoPlayer;
                            currentState = GameState::Serve;   
                        }
                        else if(exitText.getGlobalBounds().contains(mousePos))
                        {
                            window.close();
                        }
                        break;
                    }
                    case GameState::Paused:
                    {
                        if(Resume.getGlobalBounds().contains(mousePos))
                        {
                            currentState = GameState::Playing;
                        }
                        if(Restart.getGlobalBounds().contains(mousePos))
                        {
                            serveBall(left_paddle,right_paddle,ball,ballVelocityX, ballVelocityY, Center,BallSpeed,engine,flip,leftserve,rightserve,InitialBallspeed);
                            rest.restart();
                            leftScore=0;rightScore=0;
                            currentState=GameState::Serve;
                        }
                        if(MenuText.getGlobalBounds().contains(mousePos))
                        {
                            ball.setPosition({WindowWidth/2,WindowHeight/2});
                            leftScore=0;rightScore=0;
                            currentState = GameState::Menu;
                        }
                        if(exitText.getGlobalBounds().contains(mousePos))
                        {
                            window.close();
                        }
                        break;
                    }
                    case GameState::GameOver:
                    {
                        if(PlayAgain.getGlobalBounds().contains(mousePos))
                        {
                            leftScore = 0; rightScore=0;
                            serveBall(left_paddle,right_paddle,ball,ballVelocityX, ballVelocityY, Center,BallSpeed,engine,flip,leftserve,rightserve,InitialBallspeed);
                            rest.restart();
                            currentState = GameState::Serve;  
                        } 
                        else if(MenuText.getGlobalBounds().contains(mousePos))
                        {
                            currentState = GameState::Menu;
                        }
                        else if(exitText.getGlobalBounds().contains(mousePos))
                        {
                            window.close();
                        }
                        break;
                    }
                   } 
                }
            }
        }
        auto dt = clock.restart();
        float deltatime = dt.asSeconds();
        switch(currentState)
        {
            //Menu
            case GameState::Menu:
            {
                onePlayerText.setOutlineThickness(0);
                twoPlayerText.setOutlineThickness(0);
                exitText.setOutlineThickness(0);
                onePlayerText.setFillColor(sf::Color::White);
                twoPlayerText.setFillColor(sf::Color::White);
                exitText.setFillColor(sf::Color::White);
                if(onePlayerText.getGlobalBounds().contains(mousePos))
                {
                    onePlayerText.setOutlineThickness(5);
                    onePlayerText.setFillColor(sf::Color::Yellow);
                }
                if(twoPlayerText.getGlobalBounds().contains(mousePos))
                {
                    twoPlayerText.setOutlineThickness(5);
                    twoPlayerText.setFillColor(sf::Color::Yellow);
                }
                if(exitText.getGlobalBounds().contains(mousePos))
                {
                    exitText.setFillColor(sf::Color::Yellow);
                    exitText.setOutlineThickness(5);
                }
                window.clear(sf::Color(0,255,255));
                window.draw(Pong);
                window.draw(Astrath);
                window.draw(onePlayerText);
                window.draw(twoPlayerText);
                window.draw(exitText);
                window.display();
                break;
            }    
            //Serve
            case GameState::Serve:
            {
                if(rest.getElapsedTime().asSeconds()>=1)
                {
                    currentState = GameState::Playing;
                }
                window.clear(sf::Color(0,255,255));
                centerline(dash,WindowHeight,window,WindowWidth);
                window.draw(left_paddle);
                window.draw(right_paddle);
                window.draw(ball);
                window.draw(scoreText);

                window.display();
                break;
            }
            //Playing
            case GameState::Playing:
            {
                if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Escape))
                {
                    currentState = GameState::Paused;
                }
                if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W))//inpute detection
                {
                    if(left_paddle.getPosition().y>PaddleHalfHeight)
                    left_paddle.move({paddleVelocityX*deltatime,-paddleVelocityY*deltatime});
                }
                if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S))
                {
                    if(left_paddle.getPosition().y<WindowHeight-PaddleHalfHeight)
                    left_paddle.move({paddleVelocityX*deltatime,paddleVelocityY*deltatime});
                }
                if(currentMode == GameMode::TwoPlayer)
                {
                    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up))
                    {
                        if(right_paddle.getPosition().y>PaddleHalfHeight)
                        right_paddle.move({paddleVelocityX*deltatime,-paddleVelocityY*deltatime});
                    }
                    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down))
                    {
                        if(right_paddle.getPosition().y<WindowHeight-PaddleHalfHeight)
                        right_paddle.move({paddleVelocityX*deltatime,paddleVelocityY*deltatime});
                    }
                }
                auto prevballpos = ball.getPosition();
                ball.move({ballVelocityX*deltatime,ballVelocityY*deltatime});//movement of ball every iteration
                auto ballpos = ball.getPosition();
                auto leftpos = left_paddle.getPosition();
                auto rightpos = right_paddle.getPosition();
                float offshootleft = ballpos.y-leftpos.y;
                float offshootright = ballpos.y-rightpos.y;
                float normalisedoffshootleft = offshootleft/(PaddleHalfHeight+BallRadius);
                float normalisedoffshootright = offshootright/(PaddleHalfHeight+BallRadius);
                float phileft = (60.f*PI/180.f)*normalisedoffshootleft;
                float phiright = (60.f*PI/180.f)*normalisedoffshootright;
                if(currentMode == GameMode::OnePlayer)
                {
                    if(ballVelocityX>0)
                    {
                        if(ballpos.y>rightpos.y+5.f && rightpos.y<WindowHeight-PaddleHalfHeight)
                        right_paddle.move({paddleVelocityX*deltatime,paddleVelocityY*deltatime});
                        else if(ballpos.y<rightpos.y-5.f && rightpos.y>PaddleHalfHeight)
                        right_paddle.move({paddleVelocityX*deltatime,-paddleVelocityY*deltatime});
                    }
                    else
                    {
                        if(rightpos.y>(WindowHeight/2.f) +5.f && rightpos.y>PaddleHalfHeight)
                        right_paddle.move({paddleVelocityX*deltatime,-paddleVelocityY*deltatime});
                        if(rightpos.y<(WindowHeight/2.f)-5.f && rightpos.y<WindowHeight-PaddleHalfHeight)
                        right_paddle.move({paddleVelocityX*deltatime,paddleVelocityY*deltatime});
                    }
                }
                // ball with lower boundary collison
                if(ballpos.y>=WindowHeight-BallRadius)
                {
                    ball.setPosition({ballpos.x,WindowHeight-BallRadius});
                    ballVelocityY = -ballVelocityY;
                }
                //ball with upper boundary collison
                if(ballpos.y<=BallRadius)
                {
                    ball.setPosition({ballpos.x,BallRadius});
                    ballVelocityY = -ballVelocityY;
                }
                //ball and left paddle collison(right edge)
                if(ballpos.x<=leftpos.x+PaddleHalfWidth+BallRadius && prevballpos.x>leftpos.x+PaddleHalfWidth+BallRadius && ballpos.y>=leftpos.y-PaddleHalfHeight-BallRadius && ballpos.y<=leftpos.y+PaddleHalfHeight+BallRadius)
                {
                    ball.setPosition({leftpos.x+PaddleHalfWidth+BallRadius,ballpos.y});
                    BallSpeed = std::min(1.06f*BallSpeed,maxBallspeed);
                    ballVelocityX = BallSpeed*std::cos(phileft);
                    ballVelocityY = BallSpeed*std::sin(phileft);
                }
                //ball and left paddle collison(top edge)
                if(ballpos.x<=leftpos.x+PaddleHalfWidth+BallRadius && ballpos.x>=leftpos.x-PaddleHalfWidth-BallRadius && ballpos.y>=leftpos.y-PaddleHalfHeight-BallRadius && prevballpos.y<leftpos.y-PaddleHalfHeight-BallRadius)
                {
                    ball.setPosition({ballpos.x,leftpos.y-PaddleHalfHeight-BallRadius});
                    ballVelocityY = -ballVelocityY;
                }
                //ball and left paddle collison(bottom edge)
                if(ballpos.x<leftpos.x+PaddleHalfWidth+BallRadius && ballpos.x>leftpos.x-PaddleHalfWidth-BallRadius && ballpos.y<=leftpos.y+PaddleHalfHeight+BallRadius && prevballpos.y>leftpos.y+PaddleHalfHeight+BallRadius)
                {
                    ball.setPosition({ballpos.x,leftpos.y+PaddleHalfHeight+BallRadius});
                    ballVelocityY = -ballVelocityY;
                }
                //ball and right paddle collison(left edge)
                if(prevballpos.x<rightpos.x-PaddleHalfWidth-BallRadius && ballpos.x>=rightpos.x-PaddleHalfWidth-BallRadius && ballpos.y>=rightpos.y-PaddleHalfHeight-BallRadius && ballpos.y<=rightpos.y+PaddleHalfHeight+BallRadius)
                {
                    ball.setPosition({rightpos.x-PaddleHalfWidth-BallRadius,ballpos.y});
                    BallSpeed = std::min(1.06f*BallSpeed,maxBallspeed);
                    ballVelocityX = -BallSpeed*std::cos(phiright);
                    ballVelocityY = BallSpeed*std::sin(phiright);
                }
                //ball and right paddle(top edge)
                if(ballpos.x<=rightpos.x+PaddleHalfWidth+BallRadius && ballpos.x>=rightpos.x-PaddleHalfWidth-BallRadius && ballpos.y>=rightpos.y-PaddleHalfHeight-BallRadius && prevballpos.y<rightpos.y-PaddleHalfHeight-BallRadius)
                {
                    ball.setPosition({ballpos.x,rightpos.y-PaddleHalfHeight-BallRadius});
                    ballVelocityY = -ballVelocityY;
                }
                //ball and right paddle collison(bottom edge)
                if(ballpos.x<rightpos.x+PaddleHalfWidth+BallRadius && ballpos.x>rightpos.x-PaddleHalfWidth-BallRadius && ballpos.y<=rightpos.y+PaddleHalfHeight+BallRadius && prevballpos.y>rightpos.y+PaddleHalfHeight+BallRadius)
                {
                    ball.setPosition({ballpos.x,rightpos.y+PaddleHalfHeight+BallRadius});
                    ballVelocityY = -ballVelocityY;
                }
            
                //score detection(left)
                if(ballpos.x>=WindowWidth+BallRadius && leftScore < 7)
                {
                    leftScore++;
                    serveBall(left_paddle,right_paddle,ball, ballVelocityX, ballVelocityY, Center,BallSpeed,engine,flip,leftserve,rightserve,InitialBallspeed);
                    rest.restart();
                    currentState = GameState::Serve;
                }
                //score detection(right)
                if(ballpos.x<=-BallRadius && rightScore < 7)
                {
                    rightScore++;
                    serveBall(left_paddle,right_paddle,ball, ballVelocityX, ballVelocityY, Center,BallSpeed,engine,flip,leftserve,rightserve,InitialBallspeed);
                    rest.restart();
                    currentState = GameState::Serve;
                }
            
                //score update
                scoreText.setString(std::to_string(leftScore) + "       " + std::to_string(rightScore));
                scoreText.setPosition({WindowWidth/2.f - scoreText.getLocalBounds().size.x/2.f,20.f});
            
                window.clear(sf::Color(0,255,255));
                centerline(dash,WindowHeight,window,WindowWidth);
                window.draw(left_paddle);
                window.draw(right_paddle);
                window.draw(ball);
                window.draw(scoreText);

                window.display();
                if(leftScore >=7 || rightScore >= 7)
                {
                    if(leftScore>=7)
                    {
                        WinnerText.setString("Left Wins!!");
                    }
                    else
                    {
                        WinnerText.setString("Right Wins!!");
                    }
                    currentState = GameState::GameOver;
                }
                break;
            }   
            case GameState::Paused:
            {
                Resume.setFillColor(sf::Color::White);
                Restart.setFillColor(sf::Color::White);
                MenuText.setFillColor(sf::Color::White);
                exitText.setFillColor(sf::Color::White);
                Resume.setOutlineThickness(0);
                Restart.setOutlineThickness(0);
                MenuText.setOutlineThickness(0);
                exitText.setOutlineThickness(0);
                if(Resume.getGlobalBounds().contains(mousePos))
                {
                    Resume.setFillColor(sf::Color::Yellow);
                    Resume.setOutlineThickness(5);
                }
                if(Restart.getGlobalBounds().contains(mousePos))
                {
                    Restart.setFillColor(sf::Color::Yellow);
                    Restart.setOutlineThickness(5);
                }
                if(MenuText.getGlobalBounds().contains(mousePos))
                {
                    MenuText.setFillColor(sf::Color::Yellow);
                    MenuText.setOutlineThickness(5);
                }
                if(exitText.getGlobalBounds().contains(mousePos))
                {
                    exitText.setFillColor(sf::Color::Yellow);
                    exitText.setOutlineThickness(5);
                }
                window.draw(PausedOverlay);
                window.draw(PausedText);
                window.draw(Resume);
                window.draw(Restart);
                window.draw(MenuText);
                window.draw(exitText);
                window.draw(right_paddle);
                window.draw(left_paddle);
                window.draw(ball);
                centerline(dash,WindowHeight,window,WindowWidth);
                window.display();
                break;
            } 
            case GameState::GameOver:
            {
                PlayAgain.setFillColor(sf::Color::White);
                MenuText.setFillColor(sf::Color::White);
                exitText.setFillColor(sf::Color::White);
                PlayAgain.setOutlineThickness(0);
                MenuText.setOutlineThickness(0);
                exitText.setOutlineThickness(0);
                WinnerText.setPosition({(WindowWidth-WinnerText.getLocalBounds().size.x)/2,WindowHeight/7});
                if(PlayAgain.getGlobalBounds().contains(mousePos))
                {
                    PlayAgain.setOutlineThickness(5);
                    PlayAgain.setFillColor(sf::Color::Yellow);
                }
                if(MenuText.getGlobalBounds().contains(mousePos))
                {
                    MenuText.setOutlineThickness(5);
                    MenuText.setFillColor(sf::Color::Yellow);
                }
                if(exitText.getGlobalBounds().contains(mousePos))
                {
                    exitText.setFillColor(sf::Color::Yellow);
                    exitText.setOutlineThickness(5);
                }
                window.clear(sf::Color(0,220,200));
                window.draw(WinnerText);
                window.draw(PlayAgain);
                window.draw(MenuText);
                window.draw(exitText);
                window.display();
                break;
            }    
}
    }
};
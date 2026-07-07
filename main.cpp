#include <SFML/Graphics.hpp>
#include <iostream>
#include <random>
#include <optional>
#include <string>
#include <SFML/Audio.hpp>
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
sf::RectangleShape dash;

struct Raindrop
{
    sf::RectangleShape line;
    float speed;
};
struct SplashParticle
{
    sf::CircleShape shape;
    sf::Vector2f velocity;
    float lifetime;
};
void splash(std::vector<SplashParticle> &SplashParticles, sf::RenderWindow &window)
{
    for (auto &p : SplashParticles)
    {
        window.draw(p.shape);
    }
}
void drawRain(const std::vector<Raindrop>& rain,sf::RenderWindow& window)
{
    for (const auto& drop : rain)
    {
        window.draw(drop.line);
    }
}
void centerline(sf::RectangleShape &dash,const float &Windowheight, sf::RenderWindow &window,const float &WindowWidth)
{
    for(float i = dash.getSize().y/2; i<=Windowheight;i += 1.5*dash.getSize().y)
    {
        dash.setPosition({WindowWidth/2.f,i});
        window.draw(dash);
    }
}
void serveBall(sf::RectangleShape &left_paddle, sf::RectangleShape &right_paddle, sf::Sprite &bubble, float &ballVelocityX, float& ballVelocityY,const sf::Vector2f& Center, float &BallSpeed, std::mt19937& engine, std::uniform_int_distribution<int>& flip,std::uniform_real_distribution<float>&leftserve,std::uniform_real_distribution<float>&rightserve,float InitialBallspeed)
{
    bubble.setPosition(Center);
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
    constexpr float PaddleWidth  = 10.f;
    constexpr float PaddleHeight = 100.f;
    sf::Texture ballTexture;
    if(!ballTexture.loadFromFile("assets/Images/bubble2.png"))
    {
        std::cout << "Failed to load bubble image\n";
    }
    sf::RectangleShape left_paddle;
    left_paddle.setSize({10.f,100.f});
    left_paddle.setOrigin({5.f,50.f});
    left_paddle.setPosition({25.f,300.f});

    sf::RectangleShape right_paddle;
    right_paddle.setSize({10.f,100.f});
    right_paddle.setOrigin({5.f,50.f});
    right_paddle.setPosition({775.f,300.f});
    sf::Sprite bubble(ballTexture);
    left_paddle.setFillColor(sf::Color(210,220,235));
    right_paddle.setFillColor(sf::Color(210,220,235));
    left_paddle.setOutlineThickness(1.f);
    left_paddle.setOutlineColor(sf::Color(120,135,155));
    right_paddle.setOutlineThickness(1.f);
    right_paddle.setOutlineColor(sf::Color(120,135,155));
    bubble.setScale({18.f/ ballTexture.getSize().x,18.f/ballTexture.getSize().y});
    bubble.setOrigin({bubble.getLocalBounds().size.x/2.f,bubble.getLocalBounds().size.y/2.f});
    bubble.setPosition({400.f,300.f});

    sf::RectangleShape PausedOverlay;
   
    std::vector<Raindrop> rain;

    sf::CircleShape trail1;
    sf::CircleShape trail2;
    sf::CircleShape trail3;
    sf::CircleShape trail4;
    trail1.setFillColor(sf::Color(120,180,255,220));
    trail1.setRadius(6.5f);
    trail1.setOrigin({6.5f,6.5f});
    trail2.setFillColor(sf::Color(120,180,255,160));
    trail2.setRadius(6.f);
    trail2.setOrigin({6.f,6.f});
    trail3.setFillColor(sf::Color(120,180,255,110));
    trail3.setRadius(5.5f);
    trail3.setOrigin({5.5f,5.5f});
    trail4.setFillColor(sf::Color(120,180,255,70));
    trail4.setRadius(5.f);
    trail4.setOrigin({5.f,5.f});
    int framecount = 0;

    const float PI = 3.14159265358979323846f;
    const float BallRadius = 9.f;
    const float PaddleHalfWidth = PaddleWidth/2.f;
    const float PaddleHalfHeight = PaddleHeight/2.f;
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
    std::uniform_int_distribution<int> lose(0,1);
    std::uniform_real_distribution<float> rainspeed(250.f,450.f);
    std::uniform_real_distribution<float> rainx(0.f, WindowWidth);
    std::uniform_real_distribution<float> rainy(-WindowHeight, 0.f);
    std::uniform_real_distribution<float> rainwidth(1.f, 2.f);
    std::uniform_real_distribution<float> rainlength(8.f, 20.f);
    std::uniform_int_distribution<int> rainalpha(80,160);
    std::uniform_real_distribution<float> newrainy(-40.f, 0.f);
    std::uniform_real_distribution<float> angle(0.f,360.f);
    std::uniform_real_distribution<float> speed(80.f, 220.f);


    float rainrotation = 10.f;
    float wind = 2.f;
    if(BallSpeed>400 && BallSpeed<500)
    wind = 4.f;
    else if(BallSpeed>500 && BallSpeed < 600)
    wind = 8.f;
    std::uniform_real_distribution<float> windvariation(-wind,wind);

    for(int i =0; i<120;i++)
    {
        Raindrop drop;
        drop.line.setSize({rainwidth(engine),rainlength(engine)});
        drop.line.setOrigin({drop.line.getSize().x/2.f,0.f});
        drop.line.setPosition({rainx(engine),rainy(engine)});
        drop.line.setRotation(sf::degrees(rainrotation+windvariation(engine)));
        drop.line.setFillColor(sf::Color(200,225,255,rainalpha(engine)));
        drop.speed = rainspeed(engine);
        rain.push_back(drop);
    }

    bool wasHoveringOnePlayer = false;
    bool wasHoveringTwoPlayer = false;
    bool wasHoveringExit = false;

    bool wasHoveringResume = false;
    bool wasHoveringRestart = false;
    bool wasHoveringMenu = false;
    bool wasHoveringPlayAgain = false;
    
    sf::SoundBuffer paddlebuffer;
    sf::SoundBuffer wallbuffer;
    sf::SoundBuffer clickbuffer;
    sf::SoundBuffer hoverbuffer;
    sf::SoundBuffer lostbuffer;
    sf::SoundBuffer scoringbuffer;
    sf::SoundBuffer startbuffer;
    sf::SoundBuffer winbuffer;
    if(!paddlebuffer.loadFromFile("assets/Sound Effects/ball and paddle.wav"))
    {
        std::cout << "Failed to load paddle sound\n";
    }
    if(!wallbuffer.loadFromFile("assets/Sound Effects/ball and wall.wav"))
    {
        std::cout << "Failed to load wall sound\n";
    }
    if(!clickbuffer.loadFromFile("assets/Sound Effects/click.wav"))
    {
        std::cout << "Failed to load click sound\n";
    }
    if(!hoverbuffer.loadFromFile("assets/Sound Effects/hover.wav"))
    {
        std::cout << "Failed to load hover sound\n";
    }
    if(!lostbuffer.loadFromFile("assets/Sound Effects/lost.wav"))
    {
        std::cout << "Failed to load lost1 sound\n";
    }
    if(!scoringbuffer.loadFromFile("assets/Sound Effects/scoring.wav"))
    {
        std::cout << "Failed to load scoring sound\n";
    }
    if(!startbuffer.loadFromFile("assets/Sound Effects/start click.flac"))
    {
        std::cout << "Failed to start paddle sound\n";
    }
    if(!winbuffer.loadFromFile("assets/Sound Effects/win.wav"))
    {
        std::cout << "Failed to load win sound\n";
    }
    sf::Sound paddlesound(paddlebuffer);
    sf::Sound wallsound(wallbuffer);
    sf::Sound clicksound(clickbuffer);
    sf::Sound hoversound(hoverbuffer);
    sf::Sound lost1sound(lostbuffer);
    sf::Sound scoringsound(scoringbuffer);
    sf::Sound startsound(startbuffer);
    sf::Sound winsound(winbuffer);
    hoversound.setVolume(150);
    clicksound.setVolume(35);
    wallsound.setVolume(25);
    paddlesound.setVolume(60);
    scoringsound.setVolume(55);
    startsound.setVolume(60);
    winsound.setVolume(1000);
    lost1sound.setVolume(120);
    sf::Music menuMusic;
    sf::Music gameplayMusic;
    if(!menuMusic.openFromFile("assets/Music/gamemusic.mp3"))
    {
        std::cout << "Failed to load Menu Music";
    }
    if(!gameplayMusic.openFromFile("assets/Music/gamemusic.mp3"))
    {
        std::cout << "Failed to load Game Music";
    }
    menuMusic.setLooping(true);
    gameplayMusic.setLooping(true);
    float menuVolume = 40.f;
    float gameVolume = 0.f;
    float pauseVolume = 20.f;
    menuMusic.setVolume(menuVolume);
    gameplayMusic.setVolume(gameVolume);
    menuMusic.play();
    bool fadetoGame = false;
    bool fadetoMenu = false;
    sf::Texture backgroundTexture;
    if(!backgroundTexture.loadFromFile("assets/Images/back.png"))
    {
        std::cout << "Failed to load background image\n";
    }

    sf::Sprite background(backgroundTexture);
    background.setScale({WindowWidth / backgroundTexture.getSize().x,WindowHeight / backgroundTexture.getSize().y});

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
    Pong.setString("Rainy Pong");
    onePlayerText.setString("One Player");
    twoPlayerText.setString("Two Player");
    exitText.setString("Exit");
    Astrath.setFillColor(sf::Color(255,16,240));
    Pong.setOutlineThickness(7);
    Pong.setOutlineColor(sf::Color::Blue);
    Pong.setFillColor(sf::Color(0,255,255));
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
    dash.setSize({6.f,24.f});
    dash.setOrigin({3.f,12.f});
    dash.setFillColor(sf::Color(220,220,220,100));
    PausedOverlay.setSize({WindowWidth,WindowHeight});
    PausedOverlay.setFillColor(sf::Color(0,80,80,80));
    sf::Text* hoveredButton = nullptr;
    sf::Text* lastHoverButton = nullptr;
    std::vector<SplashParticle> splashParticles;

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
                            startsound.play();
                            fadetoGame = true;
                            fadetoMenu = false;
                            if(gameplayMusic.getStatus() != sf::SoundSource::Status::Playing)
                            {
                                gameplayMusic.play();
                            }
                            serveBall(left_paddle,right_paddle,bubble,ballVelocityX, ballVelocityY, Center,BallSpeed,engine,flip,leftserve,rightserve,InitialBallspeed);
                            rest.restart();
                            currentMode = GameMode::OnePlayer;
                            currentState = GameState::Serve;
                        }
                        else if(twoPlayerText.getGlobalBounds().contains(mousePos))
                        {
                            startsound.play();
                            fadetoGame = true;
                            fadetoMenu = false;
                            if(gameplayMusic.getStatus() != sf::SoundSource::Status::Playing)
                            {
                                gameplayMusic.play();
                            }
                            serveBall(left_paddle,right_paddle,bubble,ballVelocityX, ballVelocityY, Center,BallSpeed,engine,flip,leftserve,rightserve,InitialBallspeed);
                            rest.restart();
                            currentMode = GameMode::TwoPlayer;
                            currentState = GameState::Serve;   
                        }
                        else if(exitText.getGlobalBounds().contains(mousePos))
                        {
                            clicksound.play();
                            window.close();
                        }
                        break;
                    }
                    case GameState::Paused:
                    {
                        if(Resume.getGlobalBounds().contains(mousePos))
                        {
                            gameplayMusic.setVolume(gameVolume);
                            clicksound.play();
                            currentState = GameState::Playing;
                        }
                        if(Restart.getGlobalBounds().contains(mousePos))
                        {
                            gameplayMusic.setVolume(gameVolume);
                            clicksound.play();
                            serveBall(left_paddle,right_paddle,bubble,ballVelocityX, ballVelocityY, Center,BallSpeed,engine,flip,leftserve,rightserve,InitialBallspeed);
                            rest.restart();
                            leftScore=0;rightScore=0;
                            currentState=GameState::Serve;
                        }
                        if(MenuText.getGlobalBounds().contains(mousePos))
                        {
                            fadetoMenu = true;
                            fadetoGame = false;
                            if(menuMusic.getStatus() != sf::SoundSource::Status::Playing)
                            {
                                menuMusic.play();
                            }
                            clicksound.play();
                            bubble.setPosition({WindowWidth/2,WindowHeight/2});
                            leftScore=0;rightScore=0;
                            currentState = GameState::Menu;
                        }
                        if(exitText.getGlobalBounds().contains(mousePos))
                        {
                            clicksound.play();
                            window.close();
                        }
                        break;
                    }
                    case GameState::GameOver:
                    {
                        if(PlayAgain.getGlobalBounds().contains(mousePos))
                        {
                            startsound.play();
                            leftScore = 0; rightScore=0;
                            serveBall(left_paddle,right_paddle,bubble,ballVelocityX, ballVelocityY, Center,BallSpeed,engine,flip,leftserve,rightserve,InitialBallspeed);
                            rest.restart();
                            currentState = GameState::Serve;  
                        } 
                        else if(MenuText.getGlobalBounds().contains(mousePos))
                        {
                            fadetoMenu = true;
                            fadetoGame = false;
                            if(menuMusic.getStatus() != sf::SoundSource::Status::Playing)
                            {
                                menuMusic.play();
                            }
                            clicksound.play();
                            leftScore = 0; rightScore=0;
                            currentState = GameState::Menu;
                        }
                        else if(exitText.getGlobalBounds().contains(mousePos))
                        {
                            clicksound.play();
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
        bubble.rotate(sf::degrees(250.f * deltatime));
        for (auto& drop : rain)
        {
            drop.line.move({0.f, drop.speed * deltatime});
            if (drop.line.getPosition().y > WindowHeight)
            {
                drop.line.setSize({rainwidth(engine),rainlength(engine)});
                drop.line.setOrigin({drop.line.getSize().x/2.f,0.f});
                drop.line.setPosition({rainx(engine),newrainy(engine)});
                drop.line.setRotation(sf::degrees(rainrotation+windvariation(engine)));
                drop.line.setFillColor(sf::Color(200,225,255,rainalpha(engine)));
                drop.speed = rainspeed(engine);
            }
        }   
        const float fadeSpeed = 35.f; // volume per second

        if(fadetoGame)
        {
            menuVolume -= fadeSpeed * deltatime;
            gameVolume += fadeSpeed * deltatime;

            menuVolume = std::max(0.f, menuVolume);
            gameVolume = std::min(40.f, gameVolume);

            menuMusic.setVolume(menuVolume);
            gameplayMusic.setVolume(gameVolume);

            if(menuVolume <= 0.f)
            {
                menuMusic.stop();
                fadetoGame = false;
            }
        }
        if(fadetoMenu)
        {
            menuVolume += fadeSpeed * deltatime;
            gameVolume -= fadeSpeed * deltatime;

            menuVolume = std::min(40.f, menuVolume);
            gameVolume = std::max(0.f, gameVolume);

            menuMusic.setVolume(menuVolume);
            gameplayMusic.setVolume(gameVolume);

            if(gameVolume <= 0.f)
            {
                gameplayMusic.stop();
                fadetoMenu = false;
            }
        }
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
                hoveredButton = nullptr;
                if(onePlayerText.getGlobalBounds().contains(mousePos))
                {
                    hoveredButton = &onePlayerText;
                    onePlayerText.setOutlineThickness(5);
                    onePlayerText.setFillColor(sf::Color::Yellow);
                }
                else if(twoPlayerText.getGlobalBounds().contains(mousePos))
                {
                    hoveredButton = &twoPlayerText;
                    twoPlayerText.setOutlineThickness(5);
                    twoPlayerText.setFillColor(sf::Color::Yellow);
                }
                else if(exitText.getGlobalBounds().contains(mousePos))
                {
                    hoveredButton = &exitText;
                    exitText.setFillColor(sf::Color::Yellow);
                    exitText.setOutlineThickness(5);
                }
                if(hoveredButton != nullptr && hoveredButton != lastHoverButton)
                {
                    hoversound.play();
                }
                lastHoverButton=hoveredButton;
                window.clear(sf::Color(0,255,255));
                window.draw(background);
                drawRain(rain,window);
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
                window.draw(background);
                drawRain(rain,window);
                centerline(dash,WindowHeight,window,WindowWidth);
                window.draw(left_paddle);
                window.draw(right_paddle);
                window.draw(scoreText);
                window.draw(bubble);
                window.display();
                break;
            }
            //Playing
            case GameState::Playing:
            {
                if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Escape))
                {
                    gameplayMusic.setVolume(pauseVolume);
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
                auto prevballpos = bubble.getPosition();
                framecount++;
                if(framecount>1)
                {
                    trail4.setPosition(trail3.getPosition());
                    trail3.setPosition(trail2.getPosition());
                    trail2.setPosition(trail1.getPosition());
                    trail1.setPosition(prevballpos);
                    framecount = 0;
                }    
                bubble.move({ballVelocityX*deltatime,ballVelocityY*deltatime});//movement of bubble every iteration
                auto ballpos = bubble.getPosition();
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
                        if(ballpos.y>rightpos.y+12.f && rightpos.y<WindowHeight-PaddleHalfHeight)
                        right_paddle.move({paddleVelocityX*deltatime,paddleVelocityY*deltatime});
                        else if(ballpos.y<rightpos.y-12.f && rightpos.y>PaddleHalfHeight)
                        right_paddle.move({paddleVelocityX*deltatime,-paddleVelocityY*deltatime});
                    }
                    else
                    {
                        if(rightpos.y>(WindowHeight/2.f) +12.f && rightpos.y>PaddleHalfHeight)
                        right_paddle.move({paddleVelocityX*deltatime,-paddleVelocityY*deltatime});
                        if(rightpos.y<(WindowHeight/2.f)-12.f && rightpos.y<WindowHeight-PaddleHalfHeight)
                        right_paddle.move({paddleVelocityX*deltatime,paddleVelocityY*deltatime});
                    }
                }

                // bubble with lower boundary collison
                if(ballpos.y>=WindowHeight-BallRadius)
                {
                    wallsound.play();
                    bubble.setPosition({ballpos.x,WindowHeight-BallRadius});
                    ballVelocityY = -ballVelocityY;
                    for(int i=0;i<10;i++)
                    {
                        SplashParticle p;

                        p.shape.setRadius(2.f);
                        p.shape.setOrigin({2.f,2.f});
                        p.shape.setFillColor(sf::Color(180,220,255));

                        p.shape.setPosition(bubble.getPosition());

                        p.velocity.x = std::cos(angle(engine)*PI/180.f)*speed(engine);
                        p.velocity.y = std::sin(angle(engine)*PI/180.f)*speed(engine);

                        p.lifetime = 0.25f;

                        splashParticles.push_back(p);
                    }
                }
                //bubble with upper boundary collison
                if(ballpos.y<=BallRadius)
                {
                    wallsound.play();
                    bubble.setPosition({ballpos.x,BallRadius});
                    ballVelocityY = -ballVelocityY;
                    for(int i=0;i<10;i++)
                    {
                        SplashParticle p;

                        p.shape.setRadius(2.f);
                        p.shape.setOrigin({2.f,2.f});
                        p.shape.setFillColor(sf::Color(180,220,255));

                        p.shape.setPosition(bubble.getPosition());

                        p.velocity.x = std::cos(angle(engine)*PI/180.f)*speed(engine);
                        p.velocity.y = std::sin(angle(engine)*PI/180.f)*speed(engine);

                        p.lifetime = 0.25f;

                        splashParticles.push_back(p);
                    }
                }
                //bubble and left paddle collison(right edge)
                if(ballpos.x<=leftpos.x+PaddleHalfWidth+BallRadius && prevballpos.x>leftpos.x+PaddleHalfWidth+BallRadius && ballpos.y>=leftpos.y-PaddleHalfHeight-BallRadius && ballpos.y<=leftpos.y+PaddleHalfHeight+BallRadius)
                {
                    paddlesound.play();
                    bubble.setPosition({leftpos.x+PaddleHalfWidth+BallRadius,ballpos.y});
                    BallSpeed = std::min(1.06f*BallSpeed,maxBallspeed);
                    ballVelocityX = BallSpeed*std::cos(phileft);
                    ballVelocityY = BallSpeed*std::sin(phileft);
                    for(int i=0;i<10;i++)
                    {
                        SplashParticle p;

                        p.shape.setRadius(2.f);
                        p.shape.setOrigin({2.f,2.f});
                        p.shape.setFillColor(sf::Color(180,220,255));

                        p.shape.setPosition(bubble.getPosition());

                        p.velocity.x = std::cos(angle(engine)*PI/180.f)*speed(engine);
                        p.velocity.y = std::sin(angle(engine)*PI/180.f)*speed(engine);

                        p.lifetime = 0.25f;

                        splashParticles.push_back(p);
                    }
                }
                //bubble and left paddle collison(top edge)
                if(ballpos.x<=leftpos.x+PaddleHalfWidth+BallRadius && ballpos.x>=leftpos.x-PaddleHalfWidth-BallRadius && ballpos.y>=leftpos.y-PaddleHalfHeight-BallRadius && prevballpos.y<leftpos.y-PaddleHalfHeight-BallRadius)
                {
                    paddlesound.play();
                    bubble.setPosition({ballpos.x,leftpos.y-PaddleHalfHeight-BallRadius});
                    ballVelocityY = -ballVelocityY;
                    for(int i=0;i<10;i++)
                    {
                        SplashParticle p;

                        p.shape.setRadius(2.f);
                        p.shape.setOrigin({2.f,2.f});
                        p.shape.setFillColor(sf::Color(180,220,255));

                        p.shape.setPosition(bubble.getPosition());

                        p.velocity.x = std::cos(angle(engine)*PI/180.f)*speed(engine);
                        p.velocity.y = std::sin(angle(engine)*PI/180.f)*speed(engine);

                        p.lifetime = 0.25f;

                        splashParticles.push_back(p);
                    }
                }
                //bubble and left paddle collison(bottom edge)
                if(ballpos.x<leftpos.x+PaddleHalfWidth+BallRadius && ballpos.x>leftpos.x-PaddleHalfWidth-BallRadius && ballpos.y<=leftpos.y+PaddleHalfHeight+BallRadius && prevballpos.y>leftpos.y+PaddleHalfHeight+BallRadius)
                {
                    paddlesound.play();
                    bubble.setPosition({ballpos.x,leftpos.y+PaddleHalfHeight+BallRadius});
                    ballVelocityY = -ballVelocityY;
                    for(int i=0;i<10;i++)
                    {
                        SplashParticle p;

                        p.shape.setRadius(2.f);
                        p.shape.setOrigin({2.f,2.f});
                        p.shape.setFillColor(sf::Color(180,220,255));

                        p.shape.setPosition(bubble.getPosition());

                        p.velocity.x = std::cos(angle(engine)*PI/180.f)*speed(engine);
                        p.velocity.y = std::sin(angle(engine)*PI/180.f)*speed(engine);

                        p.lifetime = 0.25f;

                        splashParticles.push_back(p);
                    }
                }
                //bubble and right paddle collison(left edge)
                if(prevballpos.x<rightpos.x-PaddleHalfWidth-BallRadius && ballpos.x>=rightpos.x-PaddleHalfWidth-BallRadius && ballpos.y>=rightpos.y-PaddleHalfHeight-BallRadius && ballpos.y<=rightpos.y+PaddleHalfHeight+BallRadius)
                {
                    paddlesound.play();
                    bubble.setPosition({rightpos.x-PaddleHalfWidth-BallRadius,ballpos.y});
                    BallSpeed = std::min(1.06f*BallSpeed,maxBallspeed);
                    ballVelocityX = -BallSpeed*std::cos(phiright);
                    ballVelocityY = BallSpeed*std::sin(phiright);
                    for(int i=0;i<10;i++)
                    {
                        SplashParticle p;

                        p.shape.setRadius(2.f);
                        p.shape.setOrigin({2.f,2.f});
                        p.shape.setFillColor(sf::Color(180,220,255));

                        p.shape.setPosition(bubble.getPosition());

                        p.velocity.x = std::cos(angle(engine)*PI/180.f)*speed(engine);
                        p.velocity.y = std::sin(angle(engine)*PI/180.f)*speed(engine);

                        p.lifetime = 0.25f;

                        splashParticles.push_back(p);
                    }
                }
                //bubble and right paddle(top edge)
                if(ballpos.x<=rightpos.x+PaddleHalfWidth+BallRadius && ballpos.x>=rightpos.x-PaddleHalfWidth-BallRadius && ballpos.y>=rightpos.y-PaddleHalfHeight-BallRadius && prevballpos.y<rightpos.y-PaddleHalfHeight-BallRadius)
                {
                    paddlesound.play();
                    bubble.setPosition({ballpos.x,rightpos.y-PaddleHalfHeight-BallRadius});
                    ballVelocityY = -ballVelocityY;
                    for(int i=0;i<10;i++)
                    {
                        SplashParticle p;

                        p.shape.setRadius(2.f);
                        p.shape.setOrigin({2.f,2.f});
                        p.shape.setFillColor(sf::Color(180,220,255));

                        p.shape.setPosition(bubble.getPosition());

                        p.velocity.x = std::cos(angle(engine)*PI/180.f)*speed(engine);
                        p.velocity.y = std::sin(angle(engine)*PI/180.f)*speed(engine);

                        p.lifetime = 0.25f;

                        splashParticles.push_back(p);
                    }
                }
                //bubble and right paddle collison(bottom edge)
                if(ballpos.x<rightpos.x+PaddleHalfWidth+BallRadius && ballpos.x>rightpos.x-PaddleHalfWidth-BallRadius && ballpos.y<=rightpos.y+PaddleHalfHeight+BallRadius && prevballpos.y>rightpos.y+PaddleHalfHeight+BallRadius)
                {
                    paddlesound.play();
                    bubble.setPosition({ballpos.x,rightpos.y+PaddleHalfHeight+BallRadius});
                    ballVelocityY = -ballVelocityY;
                    for(int i=0;i<10;i++)
                    {
                        SplashParticle p;

                        p.shape.setRadius(2.f);
                        p.shape.setOrigin({2.f,2.f});
                        p.shape.setFillColor(sf::Color(180,220,255));

                        p.shape.setPosition(bubble.getPosition());

                        p.velocity.x = std::cos(angle(engine)*PI/180.f)*speed(engine);
                        p.velocity.y = std::sin(angle(engine)*PI/180.f)*speed(engine);

                        p.lifetime = 0.25f;

                        splashParticles.push_back(p);
                    }
                }
                for(auto &p : splashParticles)
                {
                    p.shape.move(p.velocity*deltatime);

                    p.lifetime -= deltatime;
                }
                splashParticles.erase(std::remove_if(splashParticles.begin(),splashParticles.end(),
                    [](const SplashParticle& p)
                    {
                        return p.lifetime <= 0.f;
                    }),
                splashParticles.end());
                //score detection(left)
                if(ballpos.x>=WindowWidth+BallRadius && leftScore < 5)
                {
                    scoringsound.play();  
                    leftScore++;
                    serveBall(left_paddle,right_paddle,bubble, ballVelocityX, ballVelocityY, Center,BallSpeed,engine,flip,leftserve,rightserve,InitialBallspeed);
                    rest.restart();
                    currentState = GameState::Serve;
                }
                //score detection(right)
                if(ballpos.x<=-BallRadius && rightScore < 5)
                {
                    scoringsound.play();  
                    rightScore++;
                    serveBall(left_paddle,right_paddle,bubble, ballVelocityX, ballVelocityY, Center,BallSpeed,engine,flip,leftserve,rightserve,InitialBallspeed);
                    rest.restart();
                    currentState = GameState::Serve;
                }
            
                //score update
                scoreText.setString(std::to_string(leftScore) + "       " + std::to_string(rightScore));
                scoreText.setPosition({WindowWidth/2.f - scoreText.getLocalBounds().size.x/2.f,20.f});
            
                window.clear(sf::Color(0,255,255));
                window.draw(background);
                drawRain(rain,window);
                centerline(dash,WindowHeight,window,WindowWidth);
                window.draw(left_paddle);
                window.draw(right_paddle);
                splash(splashParticles,window);
                window.draw(trail1);
                window.draw(trail2);
                window.draw(trail3);
                window.draw(trail4);
                window.draw(bubble);
                window.draw(scoreText);

                window.display();
                if(leftScore >=5 || rightScore >= 5)
                {
                    if(leftScore>=5)
                    {
                        winsound.play();
                        WinnerText.setString("Left Wins!!");
                    }
                    else
                    {
                        if(currentMode == GameMode::OnePlayer)
                        {
                            lost1sound.play();
                        }
                        else
                        winsound.play();  
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
                hoveredButton = nullptr;
                if(Resume.getGlobalBounds().contains(mousePos))
                {
                    hoveredButton=&Resume;
                    Resume.setFillColor(sf::Color::Yellow);
                    Resume.setOutlineThickness(5);
                }
                else if(Restart.getGlobalBounds().contains(mousePos))
                {
                    hoveredButton=&Restart;
                    Restart.setFillColor(sf::Color::Yellow);
                    Restart.setOutlineThickness(5);
                }
                else if(MenuText.getGlobalBounds().contains(mousePos))
                {
                    hoveredButton=&MenuText;
                    MenuText.setFillColor(sf::Color::Yellow);
                    MenuText.setOutlineThickness(5);
                }
                else if(exitText.getGlobalBounds().contains(mousePos))
                {
                    hoveredButton=&exitText;
                    exitText.setFillColor(sf::Color::Yellow);
                    exitText.setOutlineThickness(5);
                }
                if(hoveredButton != nullptr && hoveredButton != lastHoverButton)
                {
                    hoversound.play();
                }
                lastHoverButton=hoveredButton;
                window.clear(sf::Color(0,255,255));
                window.draw(background);
                drawRain(rain,window);
                window.draw(right_paddle);
                window.draw(left_paddle);
                window.draw(scoreText);
                window.draw(trail1);
                window.draw(trail2);
                window.draw(trail3);
                window.draw(trail4);
                splash(splashParticles,window);
                window.draw(bubble);
                centerline(dash,WindowHeight,window,WindowWidth);
                window.draw(PausedOverlay);
                window.draw(PausedText);
                window.draw(Resume);
                window.draw(Restart);
                window.draw(MenuText);
                window.draw(exitText);
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
                hoveredButton = nullptr;
                WinnerText.setPosition({(WindowWidth-WinnerText.getLocalBounds().size.x)/2,WindowHeight/7});
                if(PlayAgain.getGlobalBounds().contains(mousePos))
                {
                    hoveredButton = &PlayAgain;
                    PlayAgain.setOutlineThickness(5);
                    PlayAgain.setFillColor(sf::Color::Yellow);
                }
                else if(MenuText.getGlobalBounds().contains(mousePos))
                {
                    hoveredButton = &MenuText;
                    MenuText.setOutlineThickness(5);
                    MenuText.setFillColor(sf::Color::Yellow);
                }
                else if(exitText.getGlobalBounds().contains(mousePos))
                {
                    hoveredButton = &exitText;
                    exitText.setFillColor(sf::Color::Yellow);
                    exitText.setOutlineThickness(5);
                }
                if(hoveredButton != nullptr && hoveredButton != lastHoverButton)
                {
                    hoversound.play();
                }
                lastHoverButton=hoveredButton;
                window.clear(sf::Color(0,220,200));
                window.draw(background);
                drawRain(rain,window);
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
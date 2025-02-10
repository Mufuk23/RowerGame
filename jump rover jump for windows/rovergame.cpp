#include <SFML/Graphics.hpp>
#include <iostream>
#include <chrono>
#include <ctime>
#include <math.h>
float next_obstacle_generator(){
    int obstacle_distance_min=600;
    int obstacle_distance_max=1200;
    auto randomizer = std::chrono::system_clock::now();
    return obstacle_distance_min+fmod(rand(),obstacle_distance_max-obstacle_distance_min);
}
int main() {
    // Create a window
    sf::RenderWindow window(sf::VideoMode(1280, 720), "Jump Rower Jump");
    window.setVerticalSyncEnabled(true); 
    // Load images for dynamic UI
    sf::Texture texture1;
    texture1.loadFromFile("texture/rower2d.png", sf::IntRect(0, 0, 1196, 840));
    sf::Texture background;
    background.loadFromFile("texture/mars_terrain.png", sf::IntRect(0, 0, 1280, 720));
    sf::Texture stars;
    stars.loadFromFile("texture/stars2.png", sf::IntRect(0, 0, 1280, 720));

    sf::Texture start_button;
    start_button.loadFromFile("texture/start_button.png", sf::IntRect(0, 0, 480, 300));
    sf::Texture jump_button;
    jump_button.loadFromFile("texture/jump.png", sf::IntRect(0, 0, 480, 300));

    sf::Texture title;
    title.loadFromFile("texture/title.png", sf::IntRect(0, 0, 640, 360));
    sf::Texture crater;
    crater.loadFromFile("texture/crater_new.png", sf::IntRect(0, 0, 400, 400));
    sf::Texture drone;
    drone.loadFromFile("texture/drone.png", sf::IntRect(0, 0, 400, 400));

    sf::Texture scoreboard;
    scoreboard.loadFromFile("texture/scoreboard.png", sf::IntRect(0, 0, 480, 100));
    sf::Texture numbers[10];
    for(int j=0;j<10;j++){
    numbers[j].loadFromFile("texture/numbers.png", sf::IntRect(40+(j*55), 0, 60, 100));}


    sf::Sprite sprite(texture1); 

    sf::Sprite sprite4(stars);
    sf::Sprite sprite5(stars);

    sf::Sprite sprite2(background);
    sf::Sprite sprite3(background); 

    sf::Sprite obstacle[3]; 

    sf::Sprite button(start_button);
    sf::Sprite title_card(title);

    sf::Sprite scorething(scoreboard);
    sf::Sprite scorenumbers[5];

    scorething.setPosition(700,0.0);

    for(int j=0;j<5;j++){
        scorenumbers[j].setPosition(960+(j*64),0.0);
    }

    sprite.setScale(0.2,0.2);
    button.setScale(0.5,0.5);

   
    button.setPosition(520, 500);  // Position of the button
    title_card.setPosition(320, 100);
    bool jump=false;
    float a =-720;
    float V0=720;
   
    auto game_start = std::chrono::system_clock::now();
    auto start = std::chrono::system_clock::now();
    auto current_time = std::chrono::system_clock::now();
    double t = std::chrono::duration<double>(current_time-start).count();
    double game_t = std::chrono::duration<double>(current_time-game_start).count();
    bool game_started=false;
    double x_velocity=500;
    double x=0;
    double x_mod=0;
    float floor=360;
    double y=floor;
    float sky=50;
    sprite.setPosition(100.f,floor);
    srand(time(0));
    float obstacle_x[3];
    float next_obstacle;
    int obstacle_count=3;
    int obstacle_type[3];
    // Main window loop
    int i;
    int k;
    double oldx;
    int score=0;
    int temp;
    int temp_score;
    int skychange;
    while (window.isOpen()) {
        skychange=x/5;
        if((skychange/50)%2==0){
            sky=50+skychange%100;
        }
        else{
            sky=150-skychange%100;
        }
        if(game_started){
        current_time=std::chrono::system_clock::now();
        game_t=std::chrono::duration<double>(current_time-game_start).count();
        oldx=x;
        x=game_t*x_velocity;
        x_mod=fmod(x,1280);
        sprite4.setPosition(0-x_mod,0);
        sprite5.setPosition(1280-x_mod,0);
        sprite2.setPosition(0-x_mod,0);
        sprite3.setPosition(1280-x_mod,0);
        score=x/50;
        for(i=0;i<3;i++){
            obstacle_x[i]-=(x-oldx);
        }
        next_obstacle-=(x-oldx);
        if(obstacle_x[obstacle_count%3]<-400){
            obstacle_type[obstacle_count%3]=rand()%2;
            next_obstacle+=next_obstacle_generator();
            obstacle_x[obstacle_count%3]=next_obstacle;
            obstacle_count++;
        }
        }
        else{
            button.setTexture(start_button);
        }
        if(obstacle_x[obstacle_count%3]>-150 && obstacle_x[obstacle_count%3]<165){
            if(obstacle_type[obstacle_count%3]==1){
                if(y>=floor) game_started=false;
            }
            else{
                if(y<(sky+200)) game_started=false;
            }
        }
        temp_score=score;
        for (int j=4;j>=0;j--){
            temp=temp_score%10;
            temp_score/=10;
            scorenumbers[j].setTexture(numbers[temp]);
        }
        x_velocity=500+(x/200);

        for(k=0;k<3;k++){
        if(obstacle_type[k]==1)obstacle[k].setTexture(crater);
        else obstacle[k].setTexture(drone);

        if(obstacle_type[k]==1) obstacle[k].setPosition(obstacle_x[k],floor-50);
        else obstacle[k].setPosition(obstacle_x[k],sky);
        }

            if(jump){
                t=std::chrono::duration<double>(current_time-start).count();
                y=floor-(t*(a*t+V0));
                     sprite.setPosition(100.f,y);
                if(t>1.0){
                    jump=false;
                    sprite.setPosition(100.f,floor);
                    y=floor;
                }
            }
        
        sf::Event event;
        while (window.pollEvent(event)) {
             if (event.type == sf::Event::Closed) {
                window.close();
            }
            // Detect button click
            if (event.type == sf::Event::MouseButtonPressed) {
                if (event.mouseButton.button == sf::Mouse::Left) {
                    // Check if the mouse click is within the button bounds
                    if (button.getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y)) {
                        if(!jump && game_started){
                        jump=true;
                        start = std::chrono::system_clock::now();
                        }
                        if(!game_started){
                            next_obstacle=0;
    next_obstacle+=next_obstacle_generator();
    obstacle_x[0]=next_obstacle;
    next_obstacle+=next_obstacle_generator();
    obstacle_x[1]=next_obstacle;
    next_obstacle+=next_obstacle_generator();
    obstacle_x[2]=next_obstacle;
    obstacle_count=3;
    obstacle_type[0]=rand()%2;
    obstacle_type[1]=rand()%2;
    obstacle_type[2]=rand()%2;
    x=0;
    button.setTexture(jump_button);
    if(jump){
        jump=false;
        sprite.setPosition(100.f,floor);
        y=floor;
    }

                        game_start = std::chrono::system_clock::now();
                        game_started=true;
                        x_velocity=500;
                        }
                    }
                }
            }
        }

        // Clear the window
        window.clear(sf::Color::White);

        // Draw the button and sprite (image)

        window.draw(sprite4);
        window.draw(sprite5);


        window.draw(sprite2);
        window.draw(sprite3);

        for(k=0;k<3;k++){        window.draw(obstacle[k]);}


        window.draw(button);
        window.draw(sprite);

        window.draw(scorething);

        for(k=0;k<5;k++){        window.draw(scorenumbers[k]);}

        if(!game_started){
            window.draw(title_card);
        }
        


        // Display everything we've just drawn
        window.display();
    }

    return 0;
}

#include "flappy.hpp"

FlappyBird::FlappyBird(){
    window = std::make_shared<sf::RenderWindow>(
        sf::VideoMode(1000, 600),
        "Flappy Bird",
        sf::Style:: Titlebar | sf::Style::Close
    );
    window->setFramerateLimit(60);

    gravity = frame = {0.f};
    space = {220.f};
    count = {0};

    bg.loadFromFile("./assets/img/background.png");
    flappy.loadFromFile("./assets/img/flappy.png");
    pipe.loadFromFile("./assets/img/pipe.png");

    background = std::make_shared<sf::Sprite>();
    bird = std::make_shared<sf::Sprite>();
    pipeBottom = std::make_shared<sf::Sprite>();
    pipeTop = std::make_shared<sf::Sprite>();

    background->setTexture(bg);
    bird->setTexture(flappy);
    pipeBottom->setTexture(pipe);
    pipeTop->setTexture(pipe);

    bird->setPosition(
        500.f - flappy.getSize().x / 2.f,
        300.f - flappy.getSize().y / 2.f
    );

    bird->setScale(2.f, 2.f);
    bird->setTextureRect(sf::IntRect(0, 0, 34, 24));

    pipeBottom->setScale(1.5f, 1.5f);
    pipeTop->setScale(1.5f, -1.5f);

    gameover = add = {false};
    score = {0};

    font.loadFromFile("./assets/font/flappybird.ttf");

    txt_gameover.setFont(font);
    txt_gameover.setString("Aperte SPACE para reiniciar");
    txt_gameover.setPosition(200, 300);
    txt_gameover.setCharacterSize(50);
    txt_gameover.setOutlineThickness(3);

    txt_score.setFont(font);
    txt_score.setString(std::to_string(score));
    txt_score.setPosition(10.f, 10.f);
    txt_score.setCharacterSize(50);
    txt_score.setOutlineThickness(3);
}

void FlappyBird::events(){
    auto e = std::make_shared<sf::Event>(); //Um ponteiro inteligente de eventos
    while( window->pollEvent( *e )){ //Enquanto tiver evento pendente capturado pelo ponteiro e
        if(e->type == sf::Event::Closed || (e->type == sf::Event::KeyPressed //Se o evento for de fechar
        && e->key.code == sf::Keyboard::Escape)){ //Ou a tecla ESC foi apertada
            window->close(); //Fecha a janela do jogo
        }
    }

    if(gameover && sf::Keyboard::isKeyPressed(sf::Keyboard::Space)){ //Se o jogo acabou e foi apertado SPACE
        score = 0; //A pontuacao eh reiniciada
        txt_score.setString(std::to_string(score)); //O texto de pontuacao eh atualizado
        pipes.clear(); //Retira os canos da tela
        bird->setPosition(
            500.f - flappy.getSize().x / 2.f, //Posiciona o passaro no meio da tela (X)
            300.f - flappy.getSize().y / 2.f //(Y)
        );
        gameover = false; //Reinicia o jogo na variavel de controle booleana
    }
}

void FlappyBird::draw(){
  window->clear(sf::Color::Black); //Limpa a tela com uma cor
  window->draw(*background); //Desenha o objeto desenhavel que representa o fundo

  for(auto &p : pipes){ //
   window->draw(p); 
  }

  window->draw(*bird); //Desenha o passaro na tela

  if( gameover ){ //Se o jogador perdeu
    window->draw( txt_gameover ); //Desenha o texto na tela da janela
  }

  window->draw(txt_score); //Desenha a pontuacao na tela

  window->display(); //Mostra na tela o que foi renderizado na tela
}

void FlappyBird::run(){
  while( window->isOpen() ){ //Enquanto a janela estiver aberta
    events(); //Carrega os eventos do jogo, definidos pelos cliques e teclas
    game(); //Chama os comportamentos logicos do jogo
    draw(); //Chama os comportamentos visuais do jogo

    ++count; //Acrescenta o contador
    if( count == 300 ){
      count = 0; //Reinicia o contador quando chegar em 300
    }  
  }
}

void FlappyBird::movePipes(){

  if(sf::Mouse::isButtonPressed(sf::Mouse::Left)){ //Se o botao esquerdo do mouse foi pressionado
    gravity = -8.f; //
    bird->setRotation(-frame - 10.f); //Rotaciona o passaro 
  }else{
    bird->setRotation(frame - 10.f);
  }

  if( count % 150 == 0 ){ //A cada 150 contagens
    int pos = std::rand() % 275 + 175; //Define uma posicao randomica entre 175 e 449

    pipeBottom->setPosition(1000, pos + space); //Posiciona o cano de baixo :o
    pipeTop->setPosition(1000, pos); //Posiciona o cano do topo

    pipes.push_back(*pipeBottom); //
    pipes.push_back(*pipeTop); //
  }

  for (std::size_t i {}; i < pipes.size(); ++i) { //Para cada pipe no vetor de pipes

    if( pipes[i].getGlobalBounds().intersects(bird->getGlobalBounds())){ //Se o pipe interseccionar com o passaro
      bird->move(15.f, 0); //O passaro eh movido 15 pontos a frente na horizontal

      if( pipes[i].getScale().y < 0 ){ //Se a escala do pipe que o passaro bateu em relacao a y eh menor que 0
        bird->move(0, -15.f); //O passaro desce 15 pontos na vertical
      }else{
        bird->move(0, 15.f); //O passaro sobe 15 pontos na vertical
      }

      gameover = true; //O jogo eh parado, da gameover
    }

   if( pipes[i].getPosition().x < -100 ){
     pipes.erase(pipes.begin() + i );
   } 

   pipes[i].move(-4.f, 0);

   if(pipes[i].getPosition().x == 448 && !add ){
     //std::cout << pipes[i].getPosition().x << '\n';
     txt_score.setString(std::to_string(++score));
     add = true;
   }else{
     add = false;
   }

  }

}

void FlappyBird::game(){
  if(!gameover){ //Se o jogador nao perdeu
    setAnimeBird(); //Posiciona o passaro
    moveBird(); //O passaro se move
    movePipes(); //Os canos se movem
  }
}

void FlappyBird::setAnimeBird(){
  frame += 0.15f;

  if( frame > 3 ){
    frame -= 3;
  }

  bird->setTextureRect(sf::IntRect( 34 * (int)frame, 0, 34, 24 ));
}

void FlappyBird::moveBird(){
  bird->move(0, gravity); //Adiciona a posicao (x,y) do passaro na tela
  gravity += 0.5f; //Adiciona 0.5 a gravidade
}
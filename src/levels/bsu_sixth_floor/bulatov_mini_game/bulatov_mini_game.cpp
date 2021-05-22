#include "bulatov_mini_game.h"

#include "../../../utils/parser/ase_animation_parser.h"
#include "../../../components/components.h"
#include "../../../utils/parser/dialog_parser.h"

#include <QPainter>
#include <QSpacerItem>

#include <QDebug>

namespace game {

void BulatovMiniGame::Process() {
  drawer_->Process();
}

BulatovMiniGame::BulatovMiniGame(Callback callback, QWidget* container,
                                 ecs::World* world) {
  drawer_ = new Drawer(callback, container, world);
  drawer_->setParent(container);
  drawer_->resize(container->size());
  drawer_->show();
}

void BulatovMiniGame::Drawer::Process() {
  if (game_state_ == GameState::kStartup) {
    qDebug() << "GameState::kStartup";
    game_state_ = GameState::kQ1;
    real_state_ = GameState::kQ1;
    LoadAnimations();
    animation_player = &world_->CreateEntity()
        .AddComponent<AnimationComponent>(animations_["no_1"])
        .AddComponent<SpriteComponent>(QRect(0,0,0,0));


  } else if (game_state_ == GameState::kProcessing) {
  } else if (game_state_ == GameState::kQ1) {
    if (!dialog_finished_) {
      qDebug() << "GameState::kQ1 dialog";
      animation_player->GetComponent<AnimationComponent>().SetAnimationResource(animations_["no_1"]);
      std::vector<std::pair<QString, int>> parts = {{"Существует также близкая к интерполяции задача, которая заключается в аппроксимации какой-либо сложной функции другой, более простой функцией. Если некоторая функция слишком сложна для производительных вычислений, можно попытаться вычислить её значение в нескольких", 1000}, {"точках, а по ним построить, то есть интерполировать, более простую функцию.", 1000}, { "Разумеется, использование упрощенной функции не позволяет получить такие же точные результаты, какие давала бы первоначал"
                                                     , 1000}};
      dialog_finished_ = false;
      game_state_ = GameState::kProcessing;
      bulatov_dialog_widget_ = new ui::NPCDialog(parts,
                                          [this](){
                                          dialog_finished_ = true;
                                          game_state_ = GameState::kQ1;},
                                          bulatov_dialog_container_);
      bulatov_dialog_widget_->SetTypingStartCallback([this](){MakeSpeaking();});
      bulatov_dialog_widget_->SetTypingEndCallback([this](){MakeNotSpeaking();});
      bulatov_dialog_widget_->Start();
    } else {
      qDebug() << "GameState::kQ1 dialog end";
      choose_widget_ = new ui::ChooseWidget(choose_widget_container_);
      choose_widget_->Start("do you you do you do you?",
                            "var 1 1 1 1 1 1 1 1 1 ",
                            "var 2 2 2 2 2 2 2 2 2 ",
                            "var 3 3 3 3 3 3 3 3 3 ",
                            "var 4 4 4 4 4 4 4 4 4 ",
                            [this](){game_state_ = GameState::kQ1No;},
                            &choose_widget_return_);
      game_state_ = GameState::kProcessing;
    }
  } else if (game_state_ == GameState::kQ1No) {
    qDebug() << "GameState::kQ1No";
  } else if (game_state_ == GameState::kQ1Yes) {
    qDebug() << "GameState::kQ1Yes";
  } else if (game_state_ == GameState::kQ1NoQ2No) {
    qDebug() << "GameState::kQ1NoQ2No";
  } else if (game_state_ == GameState::kQ1NoQ2Yes) {
    qDebug() << "GameState::kQ1NoQ2Yes";
  } else if (game_state_ == GameState::kQ1YesQ2No) {
    qDebug() << "GameState::kQ1YesQ2No";
  } else if (game_state_ == GameState::kQ1YesQ2Yes) {
    qDebug() << "GameState::kQ1YesQ2Yes";
  } else if (game_state_ == GameState::kEnd) {
    qDebug() << "GameState::kEnd";
    callback_();
    close(); //??
  }
}

void BulatovMiniGame::Drawer::LoadAnimations() {
  animations_ = utils::AseAnimationParser::Parse(
      QFile(":/bulatov_chellenge.json"));
  background_ = QPixmap(":/bulatov_chellenge.png");
}

BulatovMiniGame::Drawer::Drawer(Callback callback, QWidget* container,
                                ecs::World* world)
                                : callback_(std::move(callback)),
                                  container_(container),
                                  world_(world) {
  bulatov_dialog_container_ = new QWidget(this);
  player_dialog_container_ = new QWidget(this);
  choose_widget_container_ = new QWidget(this);
}

void BulatovMiniGame::Drawer::RecalculateSizes() {
  bulatov_dialog_container_->setGeometry(static_cast<int>(0.48*container_->width()),
                                         static_cast<int>(0.33*container_->height()),
                                         static_cast<int>(0.27*container_->width()),
                                         static_cast<int>(0.20*container_->height()));
  player_dialog_container_->setGeometry(static_cast<int>(0.29*container_->width()),
                                        static_cast<int>(0.84*container_->height()),
                                        static_cast<int>(0.67*container_->width()),
                                        static_cast<int>(0.09*container_->height()));
  choose_widget_container_->setGeometry(0,
                                        0,
                                        container_->width(),
                                        container_->height());
  auto& component = animation_player->GetComponent<AnimationComponent>();
  auto frame_index =
      component.frame_index;
  current_frame_bounds_ =
      component.animation_resource->GetFrame(frame_index);
}

void BulatovMiniGame::Drawer::paintEvent(QPaintEvent* event) {
  if (game_state_ == GameState::kStartup) {
    return;
  }
  RecalculateSizes();
  QPainter painter(this);
  QRect target = {0, 0, container_->width(), container_->height()};
  painter.drawPixmap(target, background_, current_frame_bounds_);
}

void BulatovMiniGame::Drawer::resizeEvent(QResizeEvent* event) {
  resize(container_->size());
  update();
}

void BulatovMiniGame::Drawer::MakeNotSpeaking() {
  qDebug() << "make not speaking";
  switch (real_state_) {
    case GameState::kQ1: {
      animation_player->GetComponent<AnimationComponent>().
          SetAnimationResource(animations_["ozhidanie_1"]);
      // wrong animation
      break;
    }
    case GameState::kQ1Yes:
    case GameState::kQ1No: {
      animation_player->GetComponent<AnimationComponent>().
          SetAnimationResource(animations_["ozhidanie_2"]);
      break;
    }
    case GameState::kQ1YesQ2Yes:
    case GameState::kQ1YesQ2No:
    case GameState::kQ1NoQ2No:
    case GameState::kQ1NoQ2Yes: {
      animation_player->GetComponent<AnimationComponent>().
          SetAnimationResource(animations_["ozhidanie_3"]);
      break;
    }
  }
}

void BulatovMiniGame::Drawer::MakeSpeaking() {
  qDebug() << "make speaking";
  switch (real_state_) {
    case GameState::kQ1: {
      animation_player->GetComponent<AnimationComponent>().
          SetAnimationResource(animations_["no_1"]);
      // wrong animation
      break;
    }
    case GameState::kQ1Yes:
    case GameState::kQ1No: {
      animation_player->GetComponent<AnimationComponent>().
          SetAnimationResource(animations_["no_1"]);
      break;
    }
    case GameState::kQ1YesQ2Yes:
    case GameState::kQ1YesQ2No:
    case GameState::kQ1NoQ2No:
    case GameState::kQ1NoQ2Yes: {
      animation_player->GetComponent<AnimationComponent>().
          SetAnimationResource(animations_["no_1"]);
      break;
    }
  }
}

}  // namespace game
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
    LoadAnimations();
    animation_player = &world_->CreateEntity()
        .AddComponent<AnimationComponent>(animations_["no_1"])
        .AddComponent<SpriteComponent>(QRect(0,0,0,0));


  } else if (game_state_ == GameState::kProcessing) {
  } else if (game_state_ == GameState::kQ1) {
    if (!dialog_finished_) {
      qDebug() << "GameState::kQ1 dialog";
      animation_player->GetComponent<AnimationComponent>().SetAnimationResource(animations_["no_1"]);
      std::vector<std::pair<QString, int>> parts = {{"Заходит в бар улитка говорит: можно виски с колой? "
                                                     "Мы не обслуживаем улиток извини"
                                                     "И вышвырнул ее за дверь "
                                                     "Проходят дни, недели, месяца "
                                                     "Она приходит снова в этот бар "
                                                     "Надевает свою модную рубаху "
                                                     "И говорит бармену пошел нахуй ", 10000},
                                                    {"Как-то раз пришел мужик в зоомагазин "
                                                     "Есть у вас что-нибудь чтоб умело говорить? "
                                                     "Есть говорящая сороконожка "
                                                     "Приходит домой и спросил неловко "
                                                     "Гулять идем? А та молчит… "
                                                     "Гулять идем? Снова молчит. "
                                                     "Мужик всю злость, суки наебали "
                                                     "Ты не говоришь, тихо бля я обуваюсь ", 10000},
                                                    {"vla vlavl lvlalvla alvalvav asd asd asd asd asd asd asd asd as dasd asd ad as ", 10000},
                                                    {"vla vlavl lvlalvla alvalvav asd asd asd asd asd asd asd asd as dasd asd ad as ", 10000},
                                                    {"vla vlavl lvlalvla alvalvav asd asd asd asd asd asd asd asd as dasd asd ad as ", 10000},
                                                    {"vla vlavl lvlalvla alvalvav asd asd asd asd asd asd asd asd as dasd asd ad as ", 10000}};
      dialog_finished_ = false;
      game_state_ = GameState::kProcessing;
      bulatov_dialog_widget_ = new ui::NPCDialog(parts,
                                          [this](){
                                          dialog_finished_ = true;
                                          game_state_ = GameState::kQ1;},
                                          bulatov_dialog_container_);
      bulatov_dialog_widget_->Start();
    } else {
      qDebug() << "GameState::kQ1 dialog end";
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

}  // namespace game
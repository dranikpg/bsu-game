#include "chernov_mini_game.h"

#include <vector>
#include <utility>

#include "../../utils/parser/ase_animation_parser.h"
#include "../../components/components.h"
#include "../../utils/parser/dialog_parser.h"

#include <QPainter>
#include <QHBoxLayout>

namespace game {

void ChernovMiniGame::Process() {
  drawer_->Process();
}

ChernovMiniGame::ChernovMiniGame(Callback callback, QWidget* container,
                                 ecs::World* world) {
  drawer_ = new Drawer(callback, container, world);
  drawer_->setParent(container);
  drawer_->resize(container->size());
  drawer_->show();
}

void ChernovMiniGame::Drawer::Process() {
  switch (game_state_) {
    case GameState::kFirst: {
      game_state_ = GameState::kDialog;
      chernov_dialog_ = std::make_shared<ui::NPCDialog>(
          std::vector<std::pair<QString, int>>
              {{"Здесь должен был быть отличный разговорный уровень. "
                "Но что то не повезло, не фартануло. \n \n"
                "И вместо сюжетной линии...", 1000},
               {"АНЕКДОТ", 200},
               {"Едет машина скорой помощи, внутри водитель, "
                "врач, фельдшер, медсестра и студент практикант.\n \n"
                "Едут, вдруг водитель резко"
                " тормозит и начинает задыхаться.\n \n"
                "Хватается руками за горло, шипит - ужасное зрелище.\n \n"
                "Медсестра невозмутимо и твердо хватает чемоданчик"
                " с препаратами и бьет водителя по голове.\n \n"
                "Тот моментально успокаивается"
                " и машина продолжает движение.\n \n"
                "Тут фельдшер хватается за горло, падает "
                "на пол, начинает задыхаться и сипеть.\n \n"
                "Медсестра зло бьет фельдшера по голове "
                "и тот сразу оправляется и садится на место.\n \n"
                "Не прошло и минуты, как врач "
                "начинает задыхаться и брыкаться на полу.\n \n"
                "Медсестра резким ударом чемоданчика "
                "приводит врача в чувства.\n \n"
                "Обалдевший от увиденного студент спрашивает у врача:\n \n"
                "- Я учусь только на 4 курсе и все мои знания "
                "- это сплошная теория, но о "
                "подобных симптомах я даже не слышал! "
                "А метод лечения настолько странный, "
                "что я окончательно растерян, "
                "не могли бы вы мне объяснить..\n \n"
                "Врач, махая рукой:\n \n"
                "- А, не бери в голову. "
                "У медсестры муж вчера повесился, мы ее пОдКаЛыВаЕм!", 1000}},
                                          [this](){
        game_state_ = GameState::kMillionaire;
        PauseChernovPlayer();
        millionaire_->Start(GetScreenShot());
      }, chernov_dialog_container_, ui::NPCDialog::DialogType::kRightBottom);

      chernov_dialog_->SetTypingStartCallback([this](){MakeChernovSpeaking();});
      chernov_dialog_->SetTypingEndCallback([this](){MakeChernovNotSpeaking();});

      millionaire_pack_ = {"Важный статистический опрос?",
               "Ахахахах?",
               "Охохохохоохо?",
               "АХахаххохохахох?",
               "АХАХХАХАХХ?"};
      millionaire_ = std::make_shared<ui::ChooseWidget>(
          millionaire_container_,
          millionaire_pack_[0],
          millionaire_pack_[1],
          millionaire_pack_[2],
          millionaire_pack_[3],
          millionaire_pack_[4],
          [this](int val){
            game_state_ = GameState::kFirstEnd;
            MakeChernovNotSpeaking();
            UnpauseChernovPlayer();
            chosen_var_ = val;});

      chernov_dialog_->Start();
      break;
    }
    case GameState::kFirstEnd: {
      game_state_ = GameState::kDialog;
      player_dialog_ = std::make_shared<ui::NPCDialog>(
          std::vector<std::pair<QString, int>>{{
            "Внимание. Спасибо за внимание.", 1000}},
          [this](){
            game_state_ = GameState::kEnd;
          }, player_dialog_container_, ui::NPCDialog::DialogType::kLeftBottom);
      player_dialog_->Start();
      break;
    }
    case GameState::kEnd: {
      close();
      callback_();
      break;
    }
  }
}


ChernovMiniGame::Drawer::Drawer(Callback callback, QWidget* container,
                                ecs::World* world)
                                : callback_(std::move(callback)),
                                  container_(container),
                                  world_(world) {
  resize(container_->size());
  auto container_layout = new QHBoxLayout();
  container_layout->addWidget(this);
  container_layout->setSpacing(0);
  container_layout->setContentsMargins(0, 0, 0, 0);
  delete container_->layout();
  container_->setLayout(container_layout);

  auto main_layout = new QHBoxLayout(this);
  main_layout->setSpacing(0);
  main_layout->setContentsMargins(0, 0, 0, 0);
  setLayout(main_layout);

  background_ = QPixmap(":/chernov_challenge_background.png");
  chernov_animation_ = utils::AseAnimationParser::Parse(
      QFile(":/chernov.json"));
  chernov_ = QPixmap(":/chernov.png");

  player_dialog_container_ = new QWidget(this);
  chernov_dialog_container_ = new QWidget(this);
  millionaire_container_ = new QWidget(this);

  chernov_player_ = &world_->CreateEntity()
      .AddComponent<SpriteComponent>(QRect(0, 0, 0, 0))
      .AddComponent<AnimationComponent>(chernov_animation_["speak"]);

  RecalculateSizes();
}

void ChernovMiniGame::Drawer::RecalculateSizes() {
  chernov_screen_bounds_ = QRect{static_cast<int>(0.76*container_->width()),
                                 static_cast<int>(0.447*container_->height()),
                                 static_cast<int>(0.12*container_->width()),
                                 static_cast<int>(0.21*container_->height())};
  chernov_dialog_container_->setGeometry(static_cast<int>(0.484*container_->width()),
                                   static_cast<int>(0.324*container_->height()),
                                   static_cast<int>(0.275*container_->width()),
                                   static_cast<int>(0.175*container_->height()));
  player_dialog_container_->setGeometry(static_cast<int>(0.301*container_->width()),
                                   static_cast<int>(0.59*container_->height()),
                                   static_cast<int>(0.247*container_->width()),
                                   static_cast<int>(0.166*container_->height()));
  millionaire_container_->setGeometry(0, 0, width(), height());
}

void ChernovMiniGame::Drawer::paintEvent(QPaintEvent* event) {
  QPainter painter(this);
  painter.drawPixmap(QRect{0, 0, width(), height()}, background_,
                     QRect{0, 0, background_.width(), background_.height()});
  ComputeChernovBounds();
  painter.drawPixmap(chernov_screen_bounds_, chernov_, chernov_pixmap_bounds_);
}

void ChernovMiniGame::Drawer::ComputeChernovBounds() {
  if (chernov_player_ != nullptr) {
    auto& component = chernov_player_->GetComponent<AnimationComponent>();
    auto frame_index = component.frame_index;
    chernov_pixmap_bounds_ = component.animation_resource->GetFrame(frame_index);
  }
  chernov_screen_bounds_ = QRect{static_cast<int>(0.76*container_->width()),
                                 static_cast<int>(0.447*container_->height()),
                                 static_cast<int>(0.12*container_->width()),
                                 static_cast<int>(0.21*container_->height())};
}

void ChernovMiniGame::Drawer::resizeEvent(QResizeEvent* event) {
  resize(container_->size());
  RecalculateSizes();
  update();
}

QPixmap ChernovMiniGame::Drawer::GetScreenShot() {
  QPixmap pixmap(720, 512);
  QPainter painter(&pixmap);

  painter.drawPixmap(QRect{0, 0, pixmap.width(), pixmap.height()}, background_,
                     QRect{0, 0, background_.width(), background_.height()});
  QRect chernov_bounds = QRect{static_cast<int>(0.76*pixmap.width()),
                               static_cast<int>(0.447*pixmap.height()),
                               static_cast<int>(0.12*pixmap.width()),
                               static_cast<int>(0.21*pixmap.height())};
  painter.drawPixmap(chernov_bounds, chernov_, chernov_pixmap_bounds_);

  return pixmap;
}

void ChernovMiniGame::Drawer::PauseChernovPlayer() {
  chernov_player_->GetComponent<AnimationComponent>().paused = true;
}

void ChernovMiniGame::Drawer::UnpauseChernovPlayer() {
  chernov_player_->GetComponent<AnimationComponent>().paused = false;
}

void ChernovMiniGame::Drawer::MakeChernovNotSpeaking() {
  chernov_player_->GetComponent<AnimationComponent>().
      SetAnimationResource(chernov_animation_["statik"]);
}

void ChernovMiniGame::Drawer::MakeChernovSpeaking() {
  chernov_player_->GetComponent<AnimationComponent>().
      SetAnimationResource(chernov_animation_["speak"]);
}

}  // namespace game

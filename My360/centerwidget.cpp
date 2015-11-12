#include "centerwidget.h"
#include <QPalette>
#include <QStateMachine>
#include <QState>
#include <QPropertyAnimation>
#include <bottomwidget.h>
#include <QSignalTransition>

#include <lowerbottomwidget.h>
#include <lowertopwidget.h>
#include <optimizecenterwidget.h>
#include <cleancenterwidget.h>

CenterWidget::CenterWidget(QWidget *parent) :
    QWidget(parent)
{
    InitUi();
    InitConnect();
}

void CenterWidget::closeClicked()
{
    emit closeBtnClicked();
}

void CenterWidget::startAnimation()
{
        bottomWidget->startAnimation();
        topWidget->startAnimation();
}


void CenterWidget::InitAnimation()
{
    QStateMachine *topMoveMach = new QStateMachine(topWidget);
    QState *topMoveStart = new QState(topMoveMach);
    QState *topMoveEnd = new QState(topMoveMach);

    topMoveStart->assignProperty(topWidget, "AnimationPos",
                                 QPointF(0, 0));
    topMoveEnd->assignProperty(topWidget, "AnimationPos", QPointF(100, 100));

    topMoveMach->setInitialState(topMoveStart);
    QSignalTransition *trans =
            topMoveStart->addTransition(bottomWidget->getOptimizeBtn()->getButton(),
                                        SIGNAL(clicked()), topMoveEnd);
    QPropertyAnimation *topMoveAnimation = new QPropertyAnimation(topWidget, "AnimationPos");
    topMoveAnimation->setDuration(1000);
    trans->addAnimation(topMoveAnimation);

    topMoveMach->start();
}

void CenterWidget::InitUi()
{
    topWidget = new TopBaseWidget(this);
    bottomWidget = new bottomBaseWidget(this);

    topWidget->resize(900, 440);
    bottomWidget->resize(900, 160);
    bottomWidget->move(0, 440);

     lowerOptimizeCenter = new OptimizeCenterWidget(this);
     lowerOptimizeCenter->lower();
     lowerOptimizeCenter->hide();

     lowerSafeCenter = new SafeCheckCenterWidget(this);
     lowerSafeCenter->lower();
     lowerSafeCenter->hide();

     lowerCleanWidget = new CleanCenterWidget(this);
     lowerCleanWidget->lower();
     lowerCleanWidget->hide();
}

void CenterWidget::InitConnect()
{
    connect(bottomWidget->getOptimizeBtn()->getButton(), SIGNAL(clicked()),
            this, SLOT(startAnimation()));
    connect(bottomWidget->getCleanBtn()->getButton(), SIGNAL(clicked()),
            this, SLOT(startAnimation()));
    connect(bottomWidget->getSafeBtn()->getButton(), SIGNAL(clicked()),
            this, SLOT(startAnimation()));

    connect(topWidget->getTitleBar()->getCloseBtn(),
            SIGNAL(clicked()), this, SLOT(closeClicked()));

    connect(topWidget->OutAnimation(), SIGNAL(valueChanged(QVariant)),
            lowerOptimizeCenter->getTopWidget(), SLOT(update()));
    connect(topWidget->OutAnimation(), SIGNAL(valueChanged(QVariant)),
            lowerSafeCenter->getTopWidget(), SLOT(update()));
    connect(topWidget->OutAnimation(), SIGNAL(valueChanged(QVariant)),
            lowerCleanWidget->getTopWidget(), SLOT(update()));

    connect(lowerOptimizeCenter->getTopWidget()->TitleBtn(),
            SIGNAL(clicked()), this, SLOT(startAnimation()));
    connect(lowerSafeCenter->getTopWidget()->TitleBtn(),
            SIGNAL(clicked()), this, SLOT(startAnimation()));
    connect(lowerCleanWidget->getTopWidget()->TitleBtn(),
            SIGNAL(clicked()), this, SLOT(startAnimation()));

    connect(topWidget->InAnimation(), SIGNAL(finished()), lowerOptimizeCenter, SLOT(hide()));
    connect(topWidget->InAnimation(), SIGNAL(finished()), lowerSafeCenter, SLOT(hide()));
    connect(topWidget->InAnimation(), SIGNAL(finished()), lowerCleanWidget, SLOT(hide()));

    connect(bottomWidget->getOptimizeBtn()->getButton(), SIGNAL(clicked()),
            lowerOptimizeCenter, SLOT(view()));
    connect(bottomWidget->getSafeBtn()->getButton(), SIGNAL(clicked()),
            lowerSafeCenter, SLOT(view()));
    connect(bottomWidget->getCleanBtn()->getButton(), SIGNAL(clicked()),
            lowerCleanWidget, SLOT(view()));

}

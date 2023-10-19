#include "formcomplexmachines.h"
#include "ui_formcomplexmachines.h"

#include "vmtproject.h"
#include <QHBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QVBoxLayout>
#include "interfaces/IVMTAction.h"
#include "interfaces/IVMTActionController.h"
#include "vmtactions/vmtactionmachine.h"
#include "interfaces/IVMTMachine.h"

FormComplexMachines::FormComplexMachines(IVMTActionController *controller, QWidget *parent,bool navi) :
    QWidget(parent),
    ui(new Ui::FormComplexMachines),
    _controller(controller),
    _navi(navi)
{

    ui->setupUi(this);
    qDebug() << "FormComplexMachines::Begin Ctr";
    QHBoxLayout *layout = new QHBoxLayout;
    ui->_frame->setLayout(layout);

    layout->setContentsMargins(0,0,0,0);
    _tools_group = std::shared_ptr<QButtonGroup>(new QButtonGroup(ui->_frame));

    for(auto &m: VMTProject::GetInstance().GetMachines()){


        QPushButton *button = new QPushButton();
        QHBoxLayout *pLayout = new QHBoxLayout();
        QLabel *pTextLabel = new QLabel();

        button->setCheckable(true);

        pTextLabel->setText(QString(m->GetName()));
        pTextLabel->setAlignment(Qt::AlignCenter);
        pTextLabel->setWordWrap(true);
        pTextLabel->setTextInteractionFlags(Qt::NoTextInteraction);
        pTextLabel->setMouseTracking(false);
        pTextLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        button->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        pLayout->addWidget(pTextLabel);
        pLayout->setSpacing(0);
        pLayout->setMargin(0);
        pLayout->setContentsMargins(5, 0, 5, 0);
        button->setLayout(pLayout);
        button->setProperty("machine",QString(m->GetName()));
        QObject::connect(button, SIGNAL(clicked()),
                          this, SLOT(on_complex_machine_clicked()));
        _tools_group->addButton(button);
        ui->_frame->layout()->addWidget(button);
    }
    qDebug() << "FormComplexMachines::End Ctr";
}

void FormComplexMachines::on_complex_machine_clicked(){
    QPushButton *button = dynamic_cast<QPushButton*> (sender());
    if(button)
    {
        QVariant var = button->property("machine");
        QString name = var.toString();
        auto machine = VMTProject::GetInstance().GetMachine(name);
        if(machine){
            _controller->SetAction(std::shared_ptr<IVMTAction>(new VMTActionMachine(_controller,machine,_navi)));

        }
    }
}

FormComplexMachines::~FormComplexMachines()
{
    delete ui;
}

#include "VMTMachineAlpha.h"
#include "uicanvas.h"
#include "VMTComplexMachine.h"
#include <QPixmap>
#include <QDebug>

VMTMachineAlpha::VMTMachineAlpha(char sign,std::weak_ptr<VMTComplexMachine> parent) :
    VMTMachineStub(QString("lambda"), parent, IVMTMachine::MachineType::MT_WRITE,ImageType::MACHINE_LAMBDA){
    _sign = sign;
    _size = QPoint(VMTMachineStub::_st.GetCellSize().width(),VMTMachineStub::_st.GetCellSize().height());

}

void VMTMachineAlpha::Serialize(QDataStream& stream){

    VMTMachineStub::Serialize(stream);

    stream << QChar(_sign);
}

void VMTMachineAlpha::Deserialize(QDataStream& stream){
    VMTMachineStub::Deserialize(stream);

    QChar value; stream >> value;
    _sign = value.toLatin1();
}

std::shared_ptr<IVMTMachine> VMTMachineAlpha::Clone(IVMTEnvironment*){
    std::shared_ptr<IVMTMachine> result = std::shared_ptr<IVMTMachine>(new VMTMachineAlpha(_sign,_parent));

    std::dynamic_pointer_cast<VMTMachineAlpha>(result)->_center = _center;
    std::dynamic_pointer_cast<VMTMachineAlpha>(result)->_bounds = _bounds;
    std::dynamic_pointer_cast<VMTMachineAlpha>(result)->_size = _size;
    std::dynamic_pointer_cast<VMTMachineAlpha>(result)->_power = _power;

    return result;
}

void VMTMachineAlpha::ChangeAlphabit(std::shared_ptr<VMTAlphabit> alphabit){
    if(!alphabit->IsSign(_sign))
        _sign = alphabit->GetLambda();
}


void VMTMachineAlpha::DoAction(std::shared_ptr<VMTLine> line){
 line->SetSignAtMachine(_sign);
}

bool VMTMachineAlpha::IsPowerCalculator(){
    return false;
}

bool VMTMachineAlpha::IsAlphabit(){
    return true;
}

void VMTMachineAlpha::Update(IVMTEnvironment* environment){
  /*if(environment){
    QString str;
    str.append(QChar(_sign));
    if(auto ptr = environment->GetMachine().lock()){
        if (ptr->GetAlphabit()->IsLambda(_sign)) {
            _size.rx() = 32;
            _size.ry() = 32;
            _internal_bounds.setWidth(32);
            _internal_bounds.setHeight(32);

        }
        else {
            QRect bounds;
            environment->GetGraphics().CalculateTextSize(str, bounds);
            _size.rx() = bounds.width();
            _size.ry() = bounds.height();
            _internal_bounds.setWidth(bounds.width());
            _internal_bounds.setHeight(bounds.height());
        }
    }
  }*/

    VMTMachineStub::Update(environment);
}

void VMTMachineAlpha::Paint(UICanvas& canvas, const QRect& rect){
    VMTMachineStub::Paint(canvas,rect);

    QString str;
    str.append(QChar(_sign));
    if(auto ptr = _parent.lock()){
		if (ptr->GetAlphabit()->IsLambda(_sign)) {
            canvas.DrawImage(ImageType::MACHINE_LAMBDA, _internal_bounds);
		}
		else {
            canvas.DrawText(str,_center,_size.y(), this->IsSelected());
		}
    }

}

QPixmap* VMTMachineAlpha::GetImage(){
    static QPixmap image(":/Files/images/toolbars/tools/icon_lambda.gif");
    return &image;

}

const QString& VMTMachineAlpha::GetMessage(){
    static QString msg("Select sign for machine:");
    return msg;
}

IVMTAlphabitSource *VMTMachineAlpha::GetAlphabitSource(){
    return this;
}

std::shared_ptr<VMTAlphabit> VMTMachineAlpha::GetAlphabit(){

    if(auto parent_ptr = _parent.lock())
        return parent_ptr->GetAlphabit();

    qDebug() << "VMTMachineAlpha::GetAlphabit - No alphabit!";
	return std::shared_ptr<VMTAlphabit>();
}

bool VMTMachineAlpha::IsSingleChar(){
    return true;
}

bool VMTMachineAlpha::IsEnabled(char sign){
    return _sign==sign;
}

void VMTMachineAlpha::SetEnable(char sign, bool enable,IVMTEnvironment* environment){
    if(enable) _sign=sign;

    if(auto parent = _parent.lock()){
        if(environment)
        parent->Update(environment);
    }
}

QString VMTMachineAlpha::GetSign(){
    return QString(_sign);
}

VMTMachineAlpha::~VMTMachineAlpha(){
    //qDebug() << "Delete alpha";
}

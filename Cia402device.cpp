#include "Cia402device.h"

CiA402Device::CiA402Device()
{

}

long CiA402Device::SwitchOn(){

//    co_msg input;
//    co_msg output;
//    uint8_t msg_start[] = {0x81,0x07};
//    uint8_t msg_start1[] = {0x80,0x07};

//    input=interf.SetCanOpenMsg(0,0,msg_start);

//    if (interf.SendMessage(input,0)<0){
//        cout<<"Error al enviar mensaje de reinicio"<<endl;
//        return -1;
//    }
//    else{
//        if (interf.WaitForReadMessage(output,0)<0){
//            cout<<"Error al leer el mensaje de reinicio"<<endl;
//            return -1;
//        }
//        else{
//            //msg_start[0]=0x81; msg_start[1]=0x07;
//            input=interf.SetCanOpenMsg(0,0,msg_start1);

//            if (interf.SendMessage(input,0)<0){
//                cout<<"Error al enviar el mensaje de encendido"<<endl;
//                return -1;
//            }
//            else{
////                if (interf.WaitForReadMessage(output,0)<0){
////                    cout<<"Error al leer el mensaje de encendido"<<endl;
////                    return -1;
////                }
////                else{
//                    cout<<"Proceso correcto"<<endl;
//                    return 0;
//                //}
//            }
//       }
//   }
    return 0;
}

int CiA402Device::CheckStatus(){
    return 0;
}


long CiA402Device::SwitchOff()
{

    return 0;
}

double CiA402Device::GetPosition()
{
    return 0.53; //fix this

}

long CiA402Device::SetCommunications(CiA301CommPort *newCommunications)
{
    comm = newCommunications;
    return 0;
}


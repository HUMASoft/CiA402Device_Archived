#include "Cia402device.h"

long CiA402Device::Init(uint8_t new_id)
{
    //id = new_id;
    return 0;
}

CiA402Device::CiA402Device() : CiA301CommPort(1,1) //stdout
{

    comm = 1; //stdout

}

CiA402Device::CiA402Device(uint8_t new_id) : CiA301CommPort(1, new_id)
{

    comm = 1; //stdout
}

CiA402Device::CiA402Device(uint8_t new_id, int fdPort) : CiA301CommPort(fdPort, new_id)
{
    comm = fdPort;

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

int CiA402Device::CheckStatus()
{
    //uint16_t* statusp;
    uint16_t status;
    //Ask for the status word
    status = (uint16_t) ReadSDO(od::statusword);

    cout << "status word: " << status << endl;
    //Print decoded response for status word
    switch (status)
    {

    case 1: //
        break;

    //default:
    }

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

long CiA402Device::SetCommunications(int fdPort)
{
    comm = fdPort;
    return 0;
}


#include "Cia402device.h"

CiA402Device::CiA402Device()
{
    interf.Init(vector<int>(1),"/dev/can");
}

int CiA402Device::SwitchOn(){

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
    co_msg input;
    uint8_t msg_start[]={0x40,0x60,0x41,0x00};
    //Para escritura --> {0x2B,0x41,0x60,0x0};
    input.dlc_co=sizeof (msg_start);
    // input=interf.SetCanOpenMsg(7,0,msg_start);  //Fund_code=12 , nodeID=7 -->0x607
    input.id_co=2830;
    cout<<bitset<12>(input.id_co)<<endl;
    interf.SendMessage(input,0);
    interf.WaitForReadMessage(input,0);
    return 0;
}



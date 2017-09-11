//
//  main.cpp
//  Information Technology Project
//
//  Created by Yermek Sabyrzhan on 20.03.17.
//  Copyright © 2017 Yermek Sabyrzhan. All rights reserved.
//

#include <iostream>
#include <iomanip>
#include <string>
#include <cstring>
#include <fstream>
#include <algorithm>
#include <cmath>
#include <queue>
#include <cstdlib>
#include <ctime>
#include <sstream>

using namespace std;

string shanonEnc[256];


void shannon(float prob[], int l, int r){
    
    float sum_p = 0;
    float half_p = 0;
    float dev_sum = 0;
    
    if (l == r) return;
    
    for (int i = l; i <= r; i++){
        sum_p = sum_p + prob[i];
    }
    
    half_p = sum_p / 2.0;
    int position = 1;
    
    for (int i = l; i <= r; i++){
        dev_sum += prob[i];
        if (dev_sum >= half_p) {
            position = i;
            break;
        }
    }
    
    for (int i = l; i <= position; i++){
        shanonEnc[i] += "1";
    }
    
    for (int i = position + 1; i <= r; i++){
        shanonEnc[i] += "0";
    }
    
    shannon(prob, l, position);
    shannon(prob, position + 1, r);
    
}


int main() {
    char sign[256];
    int signCount[256];
    int size = 0;
    int count = 0;
    float prob[256];
    int a = (int)(' ');
    int b = (int)('\n');
    
    ifstream ifs("/Users/Tisobyn/Documents/XcodeIOS/Information Technology Project/Information Technology Project/text.txt");
    if(ifs.is_open())
    {
        
        string text;
        text.assign((istreambuf_iterator<char>(ifs.rdbuf())), istreambuf_iterator<char>());
        
        //делаем стринг аппер кейсом
        for(int i = 0; i < text.length(); i++){
            text[i] = (char)toupper(text[i]);
        }
        
        double length = double(text.length());
        
       /*в этой часте кода нахожу все буквы и количества повтарение каждой буквы. Сначала делаю так чтобы не было повтаряющих букв потом считаю сколько там букв. Одновременно с удаленнием считаю сколько раз удалил  буквы и добавляю это в signCount[]*/

        for(int i = 0; i < text.length(); i++)
        {
            bool check = true;
            for(int j = i + 1; j < text.length(); j++)
            {
                if(text[i] == text[j]) check = false;
                
            }
            if(check){
                sign[size] = text[i];
                size++;
            }
        }
        // считаю сколько
        for(int i = 0; i < sizeof(sign); i++){
            count = 0;
            for(int j = 0; j < text.length(); j++){
                if(sign[i] == text[j]){
                    count++;
                    signCount[i] = count;
                }
            }
        }
        
    /*в последущей часте кода нахожу probobility всех элементов: сначала сортирую потом пробобилити*/
  
        for(int i = 1; i < size; i++){
            for(int j = 0; j < size-i; j++){
                if(signCount[j] < signCount[j+1])
                {
                    int temp = sign[j];
                    sign[j] = sign[j+1];
                    sign[j+1] = temp;
                    
                    temp = signCount[j];
                    signCount[j] = signCount[j+1];
                    signCount[j+1] = temp;
                    
                }
            }
        }
        for(int i = 0; i < size; i++){
            prob[i] = signCount[i]/length;
        }
    
    /* в этой части я енкодинг делаю по шэнон*/
        shannon(prob, 0, 41);
        string encoded;
        
        for(int i = 0; i < length; i++){
            for(int j = 0; j < 42; j++){
                if(text[i] == sign[j]){
                    encoded = encoded + shanonEnc[j];
                }
            }
        }

   /* в этой части я энкодинг по хаммигу то есть добавляю r1, r2, r3 */
        string encodedHam;
        int a1, a2, a3, a4, r1, r2, r3;
        string c1,c2,c3;
        
        for(int i = 0; i < encoded.length(); i = i +4){
                a1= encoded[i+0] - '0';
                a2= encoded[i+1] - '0';
                a3= encoded[i+2] - '0';
                a4= encoded[i+3] - '0';
        
            
                r1=a1^a2^a3;
                r2=a2^a3^a4;
                r3=a1^a2^a4;
            
                c1=to_string((long long)r1);
                c2=to_string((long long)r2);
                c3=to_string((long long)r3);
                encodedHam=encodedHam+encoded[i+0]+encoded[i+1]+encoded[i+2]+encoded[i+3]+c1+c2+c3;

        }
        
        /* ошибка в humming Code and mistakeEncode*/
        string mistakeEncode = encodedHam;
        
        int randomSign = 0;
        for(int i = 0; i < encodedHam.length(); i = i + 7){
            randomSign = 1 + rand()%6;
            
            if(encodedHam[i+randomSign] == '0'){
                encodedHam[i+randomSign] = '1';
            } else if(encodedHam[i+randomSign] == '1'){
                encodedHam[i+randomSign] = '0';
            }
            randomSign = 0;
        }
        
        string temp = encodedHam;
        encodedHam = mistakeEncode;
        mistakeEncode = temp;
        
        /*В этой части начала дэкодинга то есть нахождение s1, s2, s3 и после этого S */
        
        int t1,t2,t3,t4,t5,t6,t7;
        int w1, w2, w3;
        string s1, s2, s3;
        string S;
        string findHaming = mistakeEncode;
        for(int i = 0; i < findHaming.length(); i = i + 7){
           	t1=findHaming[i]-'0';
            t2=findHaming[i+1]-'0';
            t3=findHaming[i+2]-'0';
            t4=findHaming[i+3]-'0';
            t5=findHaming[i+4]-'0';
            t6=findHaming[i+5]-'0';
            t7=findHaming[i+6]-'0';
            w1=t5^t1^t2^t3;
            w2=t6^t2^t3^t4;
            w3=t7^t1^t2^t4;
            
            s1=to_string((long long)w1);
            s2=to_string((long long)w2);
            s3=to_string((long long)w3);
            
            S = s1 + s2 + s3;
            if(S == "001"){
                if(findHaming[i+6]=='0')
                    findHaming[i+6]='1';
                else if(findHaming[i+6]=='1')
                    findHaming[i+6]='0';
            
            }
            if(S == "010"){
                if(findHaming[i+5]=='0')
                    findHaming[i+5]='1';
                else if(findHaming[i+5]=='1')
                    findHaming[i+5]='0';
                
            }
            if(S == "011"){
                if(findHaming[i+3]=='0')
                    findHaming[i+3]='1';
                else if(findHaming[i+3]=='1')
                    findHaming[i+3]='0';
            }
            if(S == "100"){
                if(findHaming[i+4]=='0')
                    findHaming[i+4]='1';
                else if(findHaming[i+4]=='1')
                    findHaming[i+4]='0';
            }
            if(S == "101"){
                if(findHaming[i]=='0')
                    findHaming[i]='1';
                else if(findHaming[i]=='1')
                    findHaming[i]='0';
            }
            if(S == "110"){
                if(findHaming[i+2]=='0')
                    findHaming[i+2]='1';
                else if(findHaming[i+2]=='1')
                    findHaming[i+2]='0';
            }
            if(S == "111"){
                if(findHaming[i+1]=='0')
                    findHaming[i+1]='1';
                else if(findHaming[i+1]=='1')
                    findHaming[i+1]='0';
            }
   
        }
        
        
/*в этой части кода я дэкодирую Hamming */
        string decodeHam = "";
        
        for(int i = 0; i < findHaming.length(); i = i + 7){
            decodeHam = decodeHam + findHaming[i];
            decodeHam = decodeHam + findHaming[i+1];
            decodeHam = decodeHam + findHaming[i+2];
            decodeHam = decodeHam + findHaming[i+3];
        }

        
        string decode;
        temp = "";
        
        for(int i = 0; i < decodeHam.length(); i++){
            temp = temp + decodeHam[i];
            for(int j=0;j<size;j++){
                if(temp==shanonEnc[j]){
                    decode=decode+sign[j];
                    temp = "";
                    break;
                }
            }
        }
        
  
        
/*в этой части кода вывожу все по выбору пользователя*/
        int choose = 0;
        cout << "Please choose, what you want to see" << endl;
        cout << "1. Every sign and this sign repeating and probobility" << endl;
        cout << "2. Encoding by Shanon every sign" << endl;
        cout << "3. Encoding by Shanon text" << endl;
        cout << "4. Encoding by Hamming text" << endl;
        cout << "5. Hamming text with mistake" << endl;
        cout << "6. Hamming with corrected mistake"<< endl;
        cout << "7. Decoded Hamming" << endl;
        cout << "8. Text after decoding" << endl;
        cout << "9. To exit program" << endl;
        
        
        while (choose != 9) {
            cout << "Choose the number: " << endl;
            cin >> choose;
            switch(choose){
                case 1:
                    for(int i = 0; i < size; i++){
                        if((int)(sign[i]) == a) cout << "Space :" << signCount[i] << " : " << prob[i] << endl;
                        else if ((int)(sign[i]) == b) cout << "Next Line :" << signCount[i] << " : " << prob[i] << endl;
                        else cout << sign[i] << " :" << signCount[i] << " : " << prob[i] << endl;
                    }
                    
                    break;
                case 2: for(int i = 0; i < size; i++){
                    if((int)(sign[i]) == a) cout << "Space :" << shanonEnc[i] << endl;
                    else if ((int)(sign[i]) == b) cout << "Next Line :" << shanonEnc[i]<< endl;
                    else cout << sign[i] << " :" << shanonEnc[i] << endl;
                }
                    break;
                case 3:
                    cout << "Shanon Text: " << endl;
                        cout << encoded << endl;
                    break;
                case 4:
                    cout << "Hamming Text: " << endl;
                    cout << encodedHam << endl;
                    break;
                case 5:
                    cout << "Hamming Text with Mistake: " << endl;
                    cout << mistakeEncode << endl;
                    break;
                case 6:
                    cout << "Hamming Text corrected: " << endl;
                    cout << findHaming << endl;
                    break;
                case 7:
                    cout << "Decoding Hamming" << endl;
                    cout << decodeHam << endl;
                    break;
                case 8:
                    cout << "Text after decoding" << endl;
                    cout << decode << endl;
                    break;
                default:
                    break;
                    
            }
            
        }
        
        
        
        

        
        ifs.close();
        
         return 1;
        
    }
    
    
    else{
     cerr << "Unable to open file" << endl;
        cout << "Поменяйте путь к файлу" << endl;
    }
    
    return 0;
}


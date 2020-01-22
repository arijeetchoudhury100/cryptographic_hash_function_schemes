#include<string>
#include<iostream>
#include<cmath>
#include<ctime>
#include<cstdlib>
#include<cstdio>
#include<unordered_map>
using namespace std;

string convertToBin(string s){  
    unordered_map<char, string> mp; 
    mp['0']= "0000"; 
    mp['1']= "0001"; 
    mp['2']= "0010"; 
    mp['3']= "0011"; 
    mp['4']= "0100"; 
    mp['5']= "0101"; 
    mp['6']= "0110"; 
    mp['7']= "0111"; 
    mp['8']= "1000"; 
    mp['9']= "1001"; 
    mp['A']= "1010"; 
    mp['B']= "1011"; 
    mp['C']= "1100"; 
    mp['D']= "1101"; 
    mp['E']= "1110"; 
    mp['F']= "1111"; 
    string bin=""; 
    for(int i=0; i<s.size(); i++){ 
        bin+= mp[s[i]]; 
    } 
    return bin; 
} 
string convertToHex(string s){ 
    unordered_map<string, string> mp; 
    mp["0000"]= "0"; 
    mp["0001"]= "1"; 
    mp["0010"]= "2"; 
    mp["0011"]= "3"; 
    mp["0100"]= "4"; 
    mp["0101"]= "5"; 
    mp["0110"]= "6"; 
    mp["0111"]= "7"; 
    mp["1000"]= "8"; 
    mp["1001"]= "9"; 
    mp["1010"]= "A"; 
    mp["1011"]= "B"; 
    mp["1100"]= "C"; 
    mp["1101"]= "D"; 
    mp["1110"]= "E"; 
    mp["1111"]= "F"; 
    string hex=""; 
    for(int i=0; i<s.length(); i+=4){ 
        string ch=""; 
        ch+= s[i]; 
        ch+= s[i+1]; 
        ch+= s[i+2]; 
        ch+= s[i+3]; 
        hex+= mp[ch]; 
    } 
    return hex; 
} 
char xoring(char a,char b){
	if(a == b)
		return '0';
	else return '1';
}

string feistel_cipher(string msg,string key){
    //xor with key
    int l = msg.length();
    string res = "";
    for(int i=0;i<l;i++)
        res += xoring(msg[i],key[i]);
    //pass through S-Box
    for(int i=0;i<l;i+=2){
        char y1 = xoring(msg[i],msg[i+1]);
        char y2 = msg[i+1];
        res[i] = y1;
        res[i+1] = y2;
    }
    //pass through D-box
    for(int i=0;i<l;i+=2){
        char temp = res[i];
        res[i] = res[i+1];
        res[i+1] = temp;
    }
    return res;
}

string compress(string msg,int start,int *comp_box,int m){
	string res = "";
	for(int i=0;i<m;i++)
		res += msg[comp_box[i]+start];
	return res;
}
void hashing(string bmsg,string hmsg,int n){
	float m = hmsg.length();
	float len = bmsg.length();
	int comp_box[int(m)];
	for(int i=0;i<m;i++)
		comp_box[i] = i;
	string hashed = "",compressed = "";
	for(int i = 0;i < len;i = i+n){
		if(n == m){
			hashed = feistel_cipher(hmsg,bmsg.substr(i,n));
			hmsg = hashed;
			hashed = "";
		}
		else{
			compressed = compress(bmsg,i,comp_box,m);
			hashed = feistel_cipher(hmsg,compressed);
			hmsg = hashed;
			hashed = "";
		}
	}
    hmsg = convertToHex(hmsg);	
	cout<<"hash: "<<hmsg<<endl;
}
int main(){
	srand(time(0));
	string msg,h0;
	cout<<"enter message"<<"\n";
	cin>>msg;
	float m;
	cout<<"enter vector size: ";
	cin>>m;
	string bmsg = convertToBin(msg);
	string hmsg = "";
	for(int i=0;i<m;i++)
		hmsg += char(rand()%2 + 48);
	float len = bmsg.length(),n,t;
	cout<<"enter block size: ";
	cin>>n;
	t = ceil(len/n);

	//perform padding
	if(n*t != len){
		bmsg += '1';
		for(int i=1;i<(n*t-len);i++)
			bmsg += '0';
	}

	hashing(bmsg,hmsg,n);
	return 0;
}

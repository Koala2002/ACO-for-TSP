#include<bits/stdc++.h>
#define int long long
#define pdd pair<double,double>
#define xpos first
#define ypos second
const int FPP=1000000000;//floating point precision �B�I�ƺ�� 
using namespace std;

vector<string> CityName;
vector<pdd> CityPos;
const int testTime=100;


double CityDis(pdd p1,pdd p2){
	return sqrt(pow(p1.xpos-p2.xpos,2)+pow(p1.ypos-p2.ypos,2));
}


class ACOSol{
private:
	vector<int> CityMove;
	
public:
	vector<int> Useable;
	ACOSol(){
		for(int a=0;a<CityName.size();a++)Useable.push_back(a);
		int startidx=rand()%Useable.size();
		CityMove.push_back(Useable[startidx]);	
		Useable.erase(Useable.begin()+startidx);
	}
	
	void update(int idx){CityMove.push_back(idx);}
	int getCityBack(){return CityMove.back();}
	
	double totaldis(){
		double sum=0;

		for(int a=0;a<CityMove.size();a++){
			sum+=CityDis(CityPos[CityMove[a]],CityPos[CityMove[(a+1==CityMove.size()?0:a+1)]]);
		}
		
		return sum;
	}
	
	vector<int> getSol(){return CityMove;}
};

class ACOSystem{
private:
	vector<int> bestSol;
	vector<vector<double>> prmTable;//�O���X�� 
	const double prmValue=0.001;//�@�����ʩҲ��ͪ��O���X�ƭ� 
	const double prmEva=0.9;
	const int prmFactor=1;//�O���X�v�� 
	const int disFactor=3;//�Z���v�� 
	
	double bestDis;
	
	void prmTableUpdate(vector<int> path){//�v�����s 
		for(auto &line:prmTable){
			for(auto p:line){
				p*=prmEva;
			} 
		}
	
		for(int a=0;a<path.size();a++){
			if(a==path.size()-1){
				prmTable[path[a]][path[0]]+=prmValue;
				prmTable[path[0]][path[a]]+=prmValue;
				break;
			}
			prmTable[path[a]][path[a+1]]+=prmValue;
			prmTable[path[a+1]][path[a]]+=prmValue;
		}
	}
public:
	ACOSystem(int CitySize){
		bestDis=INT_MAX;
		prmTable=vector<vector<double>>(CitySize,vector<double>(CitySize,1));
	}
	
	void construct(){
		ACOSol sol;
		while(sol.Useable.size()>0){
			int startIdx=sol.getCityBack(),nextIdx;
			vector<int> randTable;//�üƪ�
			double totalV=0,sumP=0;
			for(int a=0;a<sol.Useable.size();a++){
				int CityIdx=sol.Useable[a];
				double disValue=pow(1/CityDis(CityPos[startIdx],CityPos[CityIdx]),disFactor);
				double prm=pow(prmTable[startIdx][CityIdx],prmFactor);
				totalV+=disValue*prm;
			}
			
			int randnum=1000000*(int)rand()+(int)rand()*1000+rand();
			
			double randValue=(double)(randnum%(FPP+1))/FPP;
			
			for(int a=0;a<sol.Useable.size();a++){
				int CityIdx=sol.Useable[a];
				double disValue=pow(1/CityDis(CityPos[startIdx],CityPos[CityIdx]),disFactor);
				double prm=pow(prmTable[startIdx][CityIdx],prmFactor);
				
				sumP+=disValue*prm/totalV;
				if(sumP>=randValue){
					nextIdx=a;
					break;
				}
			}
			
			sol.update(sol.Useable[nextIdx]);
		
			sol.Useable.erase(sol.Useable.begin()+nextIdx);
		}
	
		prmTableUpdate(sol.getSol());
		double distanceOfSol=sol.totaldis();
		
		if(bestDis>distanceOfSol){
			bestSol=sol.getSol();
			bestDis=distanceOfSol;
			show();
		}
	}
	
	void show(){
		for(int a=0;a<bestSol.size();a++)cout<<CityName[bestSol[a]]<<"->";
		cout<<CityName[bestSol[0]]<<endl<<"distance�G"<<bestDis<<endl<<endl;
	}
};

void dataInput(){
	freopen("ACOinputData.txt","r",stdin);
	string dataline;
	
	while(cin>>dataline){
		stringstream ss;
		for(int a=0;a<dataline.length();a++)if(dataline[a]==',')dataline[a]=' ';
		ss<<dataline;
		string Name;
		double x,y;
		ss>>Name>>x>>y;
		CityName.push_back(Name);
		CityPos.push_back(make_pair(x,y));
	}
}

signed main(){
	srand(time(NULL));
	dataInput();

	ACOSystem acosys(CityName.size());
	for(int a=0;a<1000;a++){
		acosys.construct();
	} 
}

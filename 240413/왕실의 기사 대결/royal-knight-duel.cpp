#include <iostream>
#include <queue>
#include <algorithm>

using namespace std;

int l,n,q;
int info[41][41];
int life[31];
int r[31],c[31],h[31],w[31],k[31];
int nr[31],nc[31];
int dmg[31];
bool is_moved[31];

int dx[4]={-1,0,1,0};
int dy[4]={0,1,0,-1};

bool possible(int idx,int dir){

    for(int i=1;i<=n;i++){
        dmg[i]=0;
        is_moved[i]=false;
        nr[i]=r[i]; //검색 시작점 백업 
        nc[i]=c[i];
    }
    queue <int> q;

    q.push(idx);
    is_moved[idx]=true;

    while(!q.empty()){
        int x= q.front(); q.pop();
        
        nr[x]+= dx[dir];
        nc[x]+= dy[dir];

        if(nr[x]<1 || nc[x] <1 || nr[x]+h[x]-1 > l || nc[x]+w[x]-1 > l) {
            return false;
        }
        for(int i=nr[x];i<=nr[x]+h[x]-1;i++){
            for(int j=nc[x];j<=nc[x]+w[x]-1;j++){
                if(info[i][j]==1) {//함정 
                    dmg[x]++;
                }
                if(info[i][j]==2) {//벽 
                    return false;
                }
                
            }
        }
        //return false를 거치지 않고 여기까지 왔단 뜻은 가려는 방향 다음칸이 공간을 넘지않고 벽도 아니라는뜻, 함정이면 겹치는 만큼 계산됨  
        for(int i=1;i<=n;i++){ //어떤조각이 충돌하는지 확인하기 
            
            if(is_moved[i] || k[i]<=0)
                continue;
            if (r[i]>nr[x]+h[x]-1 || nr[x]>r[i]+h[i]-1) // 딱붙어있는게 아니거나 다음기사의 가로마지막 좌표가 밀려는 방향에 해당하지않음
                continue;
            if(c[i]> nc[x]+w[x]-1|| nc[x]>c[i]+w[i]-1)
                continue;
            is_moved[i]=true;
            q.push(i);
        
        }
    }
    dmg[idx]=0;
    return true;
}

void move(int idx, int dir){

    if(k[idx]<=0){
        return;
    }
    if(possible(idx,dir)){
        for(int i=1;i<=n;i++){
            r[i]=nr[i];
            c[i]=nc[i];
            k[i]-=dmg[i];
        }
    }

}



int main(){

    cin>>l>>n>>q;
    for(int i=1;i<=l;i++){
        for(int j=1;j<=l;j++){
            cin>> info[i][j]; //맵 
        }
    }

    for(int i=1;i<=n;i++){
        cin>> r[i]>>c[i]>>h[i]>>w[i]>>k[i];
        life[i]=k[i];
    }
    for(int i=1;i<=q;i++){
        int idx,dir;
        cin>>idx>>dir;
        move(idx,dir);
    }

    long long ans=0;
    for(int i=1;i<=n;i++){
        if(k[i]>0){
            ans+=life[i]-k[i];
        }
    }
    cout<<ans;
    return 0;
}
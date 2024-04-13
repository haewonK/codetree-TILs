#include <iostream>
#include <queue>




using namespace std;

struct POSI{
int x,y,depth;
};

int info[51][51];

int n,m,p,c,d;
int ru_r,ru_c,sr[51],sc[51];
int s_dx[4]={-1,0,1,0},s_dy[4]={0,1,0,-1};// 상우하좌 
int r_dx[8]={-1,-1,0,1,1,1,0,-1},r_dy[8]={0,1,1,1,0,-1,-1,-1};
int s_state[31]={0,};//기절여부  기절x =0 기절하면 +1, 
bool s_out[31]={false,}; //산타 out 당했는지 확인 
int pnt_s[31]={0,};
int now_r,now_c;
bool all_santa=false;
bool boom_ru_s=false;
bool boom_s_ru=false;
int check_dist(int r1, int c1,int r2,int c2){
    int dis=abs((r1-r2)*(r1-r2)+(c1-c2)*(c1-c2));
    return dis;
}

bool s_move(int idx){

    bool move_possible=false;
    //초기화 필요 
    if(s_out[idx]==true) return false;

    if(s_state[idx]>0) {
        s_state[idx]--;
        return false;
    }
    queue<int>q;

    q.push(idx);
    
        int x=q.front(); q.pop();
        int candi=1000000;
        int pri_s=4;
        int chk_sr,chk_sc;
        int dir=0;
        for(int i=0;i<4;i++){ //최단 거리구해야함 
            now_r=sr[x]+s_dx[i];
            now_c=sc[x]+s_dy[i];

            if (now_r<1||now_c<1||now_r>n||now_c>n) continue; //좌표 안넘는지 
            if(info[now_r][now_c]!=0&&info[now_r][now_c]!=-1) //빈칸 0 ,산타있으면 1, 루돌프 있으면 -1
            continue;
            int dist=check_dist(now_r,now_c,ru_r,ru_c);
            if (dist<candi){ 
                candi=dist;
                chk_sr=now_r;
                chk_sc=now_c;
                move_possible=true;
                dir=i;
            }
        }
        if(move_possible==false) return false;

        if(move_possible==true) {
            sr[x]=chk_sr;
            sc[x]=chk_sc;
            if(sr[x]==ru_r&&sc[x]==ru_c) boom_s_ru=true;
            //return true;
        }
    

    //밀리는 산타있는지 확인 

    if(boom_s_ru) {
        int k=info[ru_r][ru_c];
        pnt_s[k]+=d;
        sr[x]=chk_sr+(-1*s_dx[dir]);
        sc[x]=chk_sc+(-1*s_dy[dir]);
        while(1){
            all_santa=false;
            for(int i=1;i<=n;i++){
                if(s_out[i]==true) all_santa=true; 
            }
            if(all_santa==false) break;


            if(info[sr[x]][sc[x]]!=x&&info[sr[x]][sc[x]]!=0&&info[sr[x]][sc[x]]!=-1){ //다른 산타가 씌여진 곳 
                int tmp= info[sr[x]][sc[x]];
                sr[tmp]+=(-1*s_dx[dir]); //밀기 
                sc[tmp]+=(-1*s_dy[dir]);
                info[sr[x]][sc[x]]=x;

                if(sr[tmp]>n||sc[tmp]>n||sr[tmp]<1||sc[tmp]<1) {
                    s_out[tmp]=true;
                    break;
                }
                else {
                    x=tmp;
                }

            }
            else if(info[sr[x]][sc[x]]==0){
                info[sr[x]][sc[x]]=x;
                break;
            }
        }
    }

    return true;
}
bool ru_move(int r, int c){
    bool move_possible=false;
    bool visited[31][31]={false,};
    int candi=1000000;
    int chk_rr=0,chk_rc=0;
    int depth=0; 
    int dir=0;
    //초기화 필요 


    queue<POSI> ru_q;
    POSI target;
    POSI min;
    min.x=0;
    min.y=0;
    min.depth=-1;
    target.x=r; target.y=c;target.depth=0; 
    visited[r][c]=true;
    ru_q.push(target); //루돌프의 좌표 

    while(!ru_q.empty()){
        POSI move_ru;
        POSI T=ru_q.front(); ru_q.pop();
        if(min.depth!=-1&&min.depth<T.depth) break;

        if(info[T.x][T.y]!=0&&info[T.x][T.y]!=-1){ //좌표상 산타있는곳이면 
            if((T.x>min.x)||(T.x==min.x&&T.y>min.y)) {
                min=T;
                chk_rr=T.x;
                chk_rc=T.y;
                move_possible=true;
            }
        }
        //if(s_out[i]==true) continue; //out 이면 넘어가기 
        for(int j=0;j<=8;j++){
             
            move_ru.x=T.x+r_dx[j];
            move_ru.y=T.y+r_dy[j];
            move_ru.depth=T.depth+1;
            if (move_ru.x<1||move_ru.y<1||move_ru.x>n||move_ru.y>n) continue; //좌표 안넘는지 
            if(!visited[move_ru.x][move_ru.y]){
                
                visited[move_ru.x][move_ru.y]=true;
                ru_q.push(move_ru);
            }
        }
        
    }

    if(move_possible==false) return false;

    if(move_possible==true) {
        ru_r=chk_rr;
        ru_c=chk_rc;
        if(info[ru_r][ru_c]!=0) boom_ru_s=true;
        
    }
    //밀리는 산타있는지 확인 

    if(boom_ru_s) {
        int k=info[ru_r][ru_c];
        pnt_s[k]+=c;
        sr[k]=chk_rr+(-1*s_dx[dir]);
        sc[k]=chk_rc+(-1*s_dy[dir]);
        while(1){
            all_santa=false;
            for(int i=1;i<=n;i++){
                if(s_out[i]==true) all_santa=true; 
            }
            if(all_santa==false) break;

            if(info[sr[k]][sc[k]]!=k&&info[sr[k]][sc[k]]!=0&&info[sr[k]][sc[k]]!=-1){ //다른 산타가 씌여진 곳 
                int tmp= info[sr[k]][sc[k]];
                sr[tmp]+=(-1*s_dx[dir]); //밀기 
                sc[tmp]+=(-1*s_dy[dir]);
                info[sr[k]][sc[k]]=k;

                if(sr[tmp]>n||sc[tmp]>n||sr[tmp]<1||sc[tmp]<1) {
                    s_out[tmp]=true;
                    break;
                }
                else {
                    k=tmp;
                }

            }
            else if(info[sr[k]][sc[k]]==0){
                info[sr[k]][sc[k]]=k;
                break;
            }
        }
    }




    return true;
}


int main() {
    cin>>n>>m>>p>>c>>d;
    cin>>ru_r,ru_c;
    info[ru_r][ru_c]=-1;
    for(int i=1;i<=p;i++){
        int p_n,r,c;
        cin>>p_n>>r>>c;
        sr[p_n]=r;
        sc[p_n]=c;
        info[r][c]=p_n; // 산타면 산타 번호 넣기 
    }

for(int i=0;i<m;i++){ // m번의 턴 
    for(int j=1;j<=n;j++){
        s_move(j);
    }
    ru_move(ru_r,ru_c);


}




    return 0;
}
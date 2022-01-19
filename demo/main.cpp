#include "../common/param.h"
#include "../common/Read.h"
#include "../Estimator/HLLEstBit.h"
#include "../cSkt/CSktHLL.h"
#include "../bSkt/BSktHLL.h"
#include "../rSkt/RSktHLL.h"
using namespace std;

int main(){
    vector<pair<uint32_t, uint32_t>> Stream;
    vector<pair<uint32_t, uint32_t>> Bench;

    int pkt_num, flow_num;

    char filename[100] = "../data/kaggleData.txt";
    char resname[100] = "../data/kaggleItemSpread.txt";
    pkt_num = Read(filename,Stream);
    flow_num = ReadRes(resname, Bench);
    cout<<"***************************"<<endl;
    timespec start_time, end_time;
    long long timediff=0;
    double insert_throughput=0;
    /*************param settting**********/
    int memory = 4*1024*1024*8;//MB
    int d = 4;
    cout<<"number of d: "<<d<<endl;

    /*************result parameter***********/
    double ab_error=0,re_error=0;
    double avg_ab_error=0, avg_re_error=0;
    ofstream outFile;

    int unit_num =128;
    int unit_size = 5;

    /******************************* cSkt ***************/
    cout<<"**********cSKT algorithm"<<endl;
    auto cskt = CSktHLL(memory, unit_num,unit_size,d);
    //auto cskt = CSktLC(memory,unit_size,depth);
    clock_gettime(CLOCK_MONOTONIC,&start_time);
    for(int i=0;i<pkt_num;i++){
        cskt.Insert(Stream[i].second,Stream[i].first);
    }
    clock_gettime(CLOCK_MONOTONIC,&end_time);
    timediff = (long long)(end_time.tv_sec - start_time.tv_sec) * 1000000000LL + (end_time.tv_nsec - start_time.tv_nsec);
    insert_throughput = (double)1000.0*pkt_num / timediff;

    int query_car = 0;

    for(int i =0;i<flow_num;i++){
        query_car = cskt.PointQuery(Bench[i].first);
      //  cout<<"flowID: "<<Bench[i].first<<"\t real car: "<<Bench[i].second<<"\t estimated car: "<<query_car<<endl;
        ab_error += abs((int)Bench[i].second - (int)query_car);
        re_error += abs((int)Bench[i].second - (int)query_car) * 1.0 / Bench[i].second;
    }
    avg_ab_error = ab_error / flow_num;
    avg_re_error = re_error / flow_num;
    cout<<"Memory \t insert_th \t avg_ab_arror \t avg_re_error"<<endl;
    cout<<memory/(1024*8)<<"\t"<<insert_throughput<<"\t"<<avg_ab_error<<"\t"<<avg_re_error<<endl;

    outFile.open("result.csv",ios::app);
    outFile <<filename<<",cskthll,"<<memory/(8*1024)<<","<<d<<","<<insert_throughput
            <<","<<avg_ab_error<<","<<avg_re_error<<endl;
    outFile.close();

    /********************************** bSkt **************/
    cout<<"*********bSKT algorithm:"<<endl;
    auto bskt = BSktHLL(memory, unit_num,unit_size,d);
    clock_gettime(CLOCK_MONOTONIC,&start_time);
    for(int i=0;i<pkt_num;i++){
        bskt.Insert(Stream[i].second,Stream[i].first);
    }
    clock_gettime(CLOCK_MONOTONIC,&end_time);
    timediff = (long long)(end_time.tv_sec - start_time.tv_sec) * 1000000000LL + (end_time.tv_nsec - start_time.tv_nsec);
    insert_throughput = (double)1000.0*pkt_num / timediff;

    ab_error = 0;
    re_error = 0;
    for(int i =0;i<flow_num;i++){
        query_car = bskt.PointQuery(Bench[i].first);
      //  cout<<"flowID: "<<Bench[i].first<<"\t real car: "<<Bench[i].second<<"\t estimated car: "<<query_car<<endl;
        ab_error += abs((int)Bench[i].second - (int)query_car);
        re_error += abs((int)Bench[i].second - (int)query_car) * 1.0 / Bench[i].second;
    }
    avg_ab_error = ab_error / flow_num;
    avg_re_error = re_error / flow_num;

    cout<<"Memory \t insert_th \t avg_ab_arror \t avg_re_error"<<endl;
    cout<<memory/(8*1024)<<"\t"<<insert_throughput<<"\t"<<avg_ab_error<<"\t"<<avg_re_error<<endl;

    outFile.open("result.csv",ios::app);
    outFile <<filename<<",bskthll,"<<memory/(8*1024)<<","<<d<<","<<insert_throughput
            <<","<<avg_ab_error<<","<<avg_re_error<<endl;
    outFile.close();

    /********************************** rSkt **************/
    cout<<"*********rSKT algorithm:"<<endl;
    auto rskt = RSktHLL(memory, unit_num,unit_size,d);
    clock_gettime(CLOCK_MONOTONIC,&start_time);
    for(int i=0;i<pkt_num;i++){
        rskt.Insert(Stream[i].second,Stream[i].first);
    }
    clock_gettime(CLOCK_MONOTONIC,&end_time);
    timediff = (long long)(end_time.tv_sec - start_time.tv_sec) * 1000000000LL + (end_time.tv_nsec - start_time.tv_nsec);
    insert_throughput = (double)1000.0*pkt_num / timediff;

    ab_error = 0;
    re_error = 0;
    for(int i =0;i<flow_num;i++){
        query_car = rskt.PointQuery(Bench[i].first);
    //    cout<<"flowID: "<<Bench[i].first<<"\t real car: "<<Bench[i].second<<"\t estimated car: "<<query_car<<endl;
        ab_error += abs((int)Bench[i].second - (int)query_car);
        re_error += abs((int)Bench[i].second - (int)query_car) * 1.0 / Bench[i].second;
    }
    avg_ab_error = ab_error / flow_num;
    avg_re_error = re_error / flow_num;

    cout<<"Memory \t insert_th \t avg_ab_arror \t avg_re_error"<<endl;
    cout<<memory/(8*1024)<<"\t"<<insert_throughput<<"\t"<<avg_ab_error<<"\t"<<avg_re_error<<endl;

    outFile.open("result.csv",ios::app);
    outFile <<filename<<",rskthll,"<<memory/(8*1024)<<","<<d<<","<<insert_throughput
            <<","<<avg_ab_error<<","<<avg_re_error<<endl;
    outFile.close();

}

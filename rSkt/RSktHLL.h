#ifndef RSKTHLL_H
#define RSKTHLL_H

class RSktHLL{
private:
    HLLEst **p_HLL;
    HLLEst **c_HLL;
    uint32_t seed[10];
    uint32_t g_seed;
    int hash_num;
    int unit_num;
    int unit_size;
    int est_number;

public:
    RSktHLL(int Mem, int Unum, int Usize, int D){
        hash_num = D;
        unit_num = Unum;
        unit_size = Usize;
        est_number = Mem / (2*unit_size*unit_num );
        cout <<"est number: "<<est_number<<endl;
        //init HLL estimator
        p_HLL = (HLLEst **) malloc(sizeof(HLLEst **) * est_number);
        for(int i=0;i<est_number;i++){
            p_HLL[i] = newHLLEst(unit_num, unit_size);
        }
        c_HLL = (HLLEst **) malloc(sizeof(HLLEst **) * est_number);
        for(int i=0;i<est_number;i++){
            c_HLL[i] = newHLLEst(unit_num,unit_size);
        }
        for(int i=0;i<hash_num;i++){
            seed[i] = i;
        }
        g_seed = 1239;

    }

    void Insert(uint32_t flowID, uint32_t elemID){
        int res;
        int g_flag;
        for(int i=0;i<hash_num;i++){
            res = murmurhash((const char*)&flowID, 4,seed[i]) % est_number;
            g_flag = murmurhash((const char*)&flowID,4,g_seed) % 2;
            if( g_flag == 0){
                HLLEstInsert(p_HLL[res],elemID);
            }else{
                HLLEstInsert(c_HLL[res], elemID);
            }
        }
    }

    int PointQuery(uint32_t flowID){
        //first find pos, whose sum of two estimator is smallest;
        int pos;
        int sum = 1<<30;
        int temp = 0;
        int res;
        int value;
        int p_val,c_val;
        for(int i=0;i<hash_num;i++){
            res = murmurhash((const char*)&flowID, 4,seed[i]) % est_number;
            p_val = (int) HLLEstGetCar(p_HLL[res]);
            c_val = (int) HLLEstGetCar(c_HLL[res]);
            temp = p_val + c_val;
            if(temp <= sum){
                sum = temp;
                pos = i;
            }
        }

        res = murmurhash((const char*)&flowID, 4,seed[pos]) % est_number;
        int g_flag = murmurhash((const char*)&flowID,4,g_seed) % 2;
        if( g_flag == 0){
            value = p_val - c_val;
        }else{
            value = c_val - p_val;
        }

        if(value <=0){
    //        cout<<"how to deal?"<<endl;
            value=1;
        }
        return value;
    }
};
#endif

#ifndef CSKTHLL_H
#define CSKTHLL_H

class CSktHLL{
private:
    HLLEst **HLL;
    uint32_t seed[10];
    int hash_num;
    int unit_size;
    int unit_num;
    //sketch
    int est_number;
    int sk_depth;
    int sk_width;
public:
    CSktHLL(int Mem, int Unum, int Usize, int Depth){
        sk_depth = Depth;
        unit_num = Unum;
        unit_size = Usize;
        est_number = Mem / (unit_num*unit_size );
        cout <<"est number: "<<est_number<<endl;
        HLL = (HLLEst **)malloc(sizeof(HLLEst **) * est_number);
        for(int i=0;i<est_number;i++){
            HLL[i] = newHLLEst(unit_num, unit_size);
        }
        sk_width = est_number / sk_depth;
        for(int i=0;i<Depth;i++){seed[i] = i;}
    }

    void Insert(uint32_t flowID, uint32_t elemID){
        int res;
     //   int tmp_level;
        for(int i=0;i<sk_depth;i++){
            res = murmurhash((const char*)&flowID,4,seed[i]) %sk_width;
            res += i*sk_width;
            HLLEstInsert(HLL[res],elemID);
        }
    }
    uint32_t PointQuery(uint32_t flowID){
        int val = 1<<20;
        int res;
        int col_res;
        for(int i=0;i<sk_depth;i++){
            res = murmurhash((const char*)&flowID,4,seed[i]) % sk_width;
            res += i*sk_width;
            col_res = (int) HLLEstGetCar(HLL[res]);
            val = min(col_res,val);
        }
        return (uint32_t)val;
    }
};
#endif

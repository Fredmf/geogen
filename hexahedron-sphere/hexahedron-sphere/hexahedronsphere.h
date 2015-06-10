//
//  hexahedronsphere.h
//  hexahedron-sphere
//
//  Created by Grin on 01.03.15.
//  Copyright (c) 2015 FredMF. All rights reserved.
//

#ifndef hexahedron_sphere_hexahedronsphere_h
#define hexahedron_sphere_hexahedronsphere_h

#include <iostream>
#include <string>
#include <fstream>
#include <cmath>

class hexahedronsphere{
private:
    
    
    struct vert{
        float x,y,z;
        vert(float X, float Y, float Z) : x(X),y(Y),z(Z){}
        void print(){
            std::cout << "v " << x << " " << y << " " << z << std::endl;
        }
        vert add(vert in){
            return (vert){x+in.x,y+in.y,z+in.z};
        }
        vert dif(float in){
            return (vert){x/in,y/in,z/in};
        }
        vert mul(float in){
            return (vert){x*in,y*in,z*in};
        }
        vert mid(vert in){//direct line
            return add(in).dif(2.0);
        }
        void save(std::ofstream *file){
            *file << "v " << x << " " << y << " " << z << std::endl;
        }
        float getLen(){
            return sqrt(x*x+y*y+z*z);
        }
        void normalize(){
            vert tmp = (vert){x,y,z};
            tmp = tmp.dif(tmp.getLen());
            x=tmp.x;
            y=tmp.y;
            z=tmp.z;
        }
        vert getNormalized(){
            vert tmp = (vert){x,y,z};
            tmp = tmp.dif(tmp.getLen());
            return tmp;
        }
        void setLenght(float len){
            vert tmp = (vert){x,y,z};
            tmp.normalize();
            tmp.mul(len);
            x=tmp.x;
            y=tmp.y;
            z=tmp.z;
        }
    };
    
    //quad as two triangles, clockwise
    /*
     a->b
     |
     v
     d<-c
     */
    struct quad{
        vert a,b,c,d;
        unsigned int x,y; // gridpos
        unsigned int indices_t[6] = {1,4,3,3,2,1};
        bool sub;
        struct quad *qa,*qb,*qc,*qd;
        quad(vert A,vert B,vert C,vert D, unsigned int in_x = 0, unsigned int in_y = 0) : a(A),b(B),c(C),d(D),sub(false),qa(nullptr),qb(nullptr),qc(nullptr),qd(nullptr),x(in_x),y(in_y){
            a.normalize();
            b.normalize();
            c.normalize();
            d.normalize();
        }
        ~quad(){
            if (sub){
                delete qa;
                delete qb;
                delete qc;
                delete qd;
            }
        }
        void print(){
            if(sub){
                qa->print();
                qb->print();
                qc->print();
                qd->print();
            }else{
                a.print();
                b.print();
                c.print();
                d.print();
            }
        }
        void printInd(unsigned int offset){
            if(sub){
                qa->printInd(offset*4);
                qb->printInd(offset*4+1);
                qc->printInd(offset*4+2);
                qd->printInd(offset*4+3);
            }else{
                unsigned int fullOffset = offset*4;
                std::cout << "f " << indices_t[0]+fullOffset << " " << indices_t[1]+fullOffset << " " << indices_t[2]+fullOffset << std::endl;
                std::cout << "f " << indices_t[3]+fullOffset << " " << indices_t[4]+fullOffset << " " << indices_t[5]+fullOffset << std::endl;
            }
        }
        void save_v(std::ofstream *file){
            if(sub){
                qa->save_v(file);
                qb->save_v(file);
                qc->save_v(file);
                qd->save_v(file);
            }else{
                a.save(file);
                b.save(file);
                c.save(file);
                d.save(file);
            }
        }
        void save_i(std::ofstream *file, unsigned int offset){
            if(sub){
                qa->save_i(file,offset*4);
                qb->save_i(file,offset*4+1);
                qc->save_i(file,offset*4+2);
                qd->save_i(file,offset*4+3);
            }else{
                unsigned int fullOffset = offset*4;
                *file << "f " << indices_t[0]+fullOffset << " " << indices_t[1]+fullOffset << " " << indices_t[2]+fullOffset << std::endl;
                *file << "f " << indices_t[3]+fullOffset << " " << indices_t[4]+fullOffset << " " << indices_t[5]+fullOffset << std::endl;
            }
        }
        void subdiv(){
            if (sub){
                qa->subdiv();
                qb->subdiv();
                qc->subdiv();
                qd->subdiv();
            }else{
                vert ab = a.getNormalized().add(b.getNormalized()).getNormalized();
                vert bc = b.getNormalized().add(c.getNormalized()).getNormalized();
                vert cd = c.getNormalized().add(d.getNormalized()).getNormalized();
                vert da = d.getNormalized().add(a.getNormalized()).getNormalized();
                vert center = ab.getNormalized().add(cd.getNormalized()).getNormalized();
                
                qa=new quad(a,ab,center,da,x*2,y*2);
                qb=new quad(ab,b,bc,center,x*2+1,y*2);
                qc=new quad(center,bc,c,cd,x*2+1,y*2+1);
                qd=new quad(da,center,cd,d,x*2,y*2+1);
                sub=true;
            }
        }
    };
    
    struct hexahedron{
        quad a,b,c,d,e,f;
        hexahedron() :
        a(
          quad(
               (vert){1.0,1.0,1.0},
               (vert){1.0,1.0,-1.0},
               (vert){1.0,-1.0,-1.0},
               (vert){1.0,-1.0,1.0}
               )
          ),
        b(
          quad(
               (vert){1.0,1.0,-1.0},
               (vert){-1.0,1.0,-1.0},
               (vert){-1.0,-1.0,-1.0},
               (vert){1.0,-1.0,-1.0}
               )
          ),
        c(
          quad(
               (vert){-1.0,1.0,-1.0},
               (vert){-1.0,1.0,1.0},
               (vert){-1.0,-1.0,1.0},
               (vert){-1.0,-1.0,-1.0}
               )
          ),
        d(
          quad(
               (vert){-1.0,1.0,1.0},
               (vert){1.0,1.0,1.0},
               (vert){1.0,-1.0,1.0},
               (vert){-1.0,-1.0,1.0}
               )
          ),
        e(
          quad(
               (vert){-1.0,1.0,-1.0},
               (vert){1.0,1.0,-1.0},
               (vert){1.0,1.0,1.0},
               (vert){-1.0,1.0,1.0}
               )
          ),
        f(
          quad(
               (vert){-1.0,-1.0,1.0},
               (vert){1.0,-1.0,1.0},
               (vert){1.0,-1.0,-1.0},
               (vert){-1.0,-1.0,-1.0}
               )
          )
        {}
        void print(){
            a.print();
            b.print();
            c.print();
            d.print();
            e.print();
            f.print();
        }
        void printInd(){
            a.printInd(0);
            b.printInd(1);
            c.printInd(2);
            d.printInd(3);
            e.printInd(4);
            f.printInd(5);
        }
        void subdiv(){
            a.subdiv();
            b.subdiv();
            c.subdiv();
            d.subdiv();
            e.subdiv();
            f.subdiv();
        }
        void save(std::ofstream *file){
            std::cout << "start" << std::endl;
            a.save_v(file);
            std::cout << ".";
            b.save_v(file);
            std::cout << ".";
            c.save_v(file);
            std::cout << ".";
            d.save_v(file);
            std::cout << ".";
            e.save_v(file);
            std::cout << ".";
            f.save_v(file);
            std::cout << ".";
            a.save_i(file,0);
            std::cout << ".";
            b.save_i(file,1);
            std::cout << ".";
            c.save_i(file,2);
            std::cout << ".";
            d.save_i(file,3);
            std::cout << ".";
            e.save_i(file,4);
            std::cout << ".";
            f.save_i(file,5);
            std::cout << "." << std::endl;
        }
    };
    
public:
    hexahedron data;
    void printVert(){
        data.print();
    }
    void printInd(){
        data.printInd();
    }
    void save(std::string filename){
        std::ofstream file;
        file.open(filename.c_str(),std::ofstream::out | std::ofstream::trunc);
        data.save(&file);
        file.close();
    }
    unsigned int subdiv(unsigned int level = 1){
        for (unsigned int i=0;i<level;i++){
            data.subdiv();
        }
        return 1;
    }
};


#endif

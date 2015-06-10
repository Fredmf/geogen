
//
//  octahedronsphere.h
//  octahedron-sphere
//
//  Created by Grin on 01.03.15.
//  Copyright (c) 2015 FredMF. All rights reserved.
//

#ifndef octahedron_sphere_octahedronsphere_h
#define octahedron_sphere_octahedronsphere_h

#include <iostream>
#include <string>
#include <fstream>
#include <cmath>

class octahedronsphere{
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
    
    //triangle, clockwise
    /*
     b->
     ^  |
     |  v
     a<-c
     */
    struct triangle{
        vert a,b,c;
        unsigned int indices_t[3] = {1,2,3};
        bool sub;
        struct triangle *qa,*qb,*qc,*qd;
        triangle(vert A,vert B,vert C, unsigned int in_x = 0, unsigned int in_y = 0) : a(A),b(B),c(C),sub(false),qa(nullptr),qb(nullptr),qc(nullptr),qd(nullptr){
            a.normalize();
            b.normalize();
            c.normalize();
        }
        ~triangle(){
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
            }
        }
        void printInd(unsigned int offset){
            if(sub){
                qa->printInd(offset*4);
                qb->printInd(offset*4+1);
                qc->printInd(offset*4+2);
                qd->printInd(offset*4+3);
            }else{
                unsigned int fullOffset = offset*3;
                std::cout << "f " << indices_t[0]+fullOffset << " " << indices_t[1]+fullOffset << " " << indices_t[2]+fullOffset << std::endl;
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
            }
        }
        void save_i(std::ofstream *file, unsigned int offset){
            if(sub){
                qa->save_i(file,offset*4);
                qb->save_i(file,offset*4+1);
                qc->save_i(file,offset*4+2);
                qd->save_i(file,offset*4+3);
            }else{
                unsigned int fullOffset = offset*3;
                *file << "f " << indices_t[0]+fullOffset << " " << indices_t[1]+fullOffset << " " << indices_t[2]+fullOffset << std::endl;
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
                vert ca = c.getNormalized().add(a.getNormalized()).getNormalized();
                
                qa=new triangle(a,ab,ca);
                qb=new triangle(ab,b,bc);
                qc=new triangle(ca,bc,c);
                qd=new triangle(ab,bc,ca);
                sub=true;
            }
        }
    };
    
    struct octahedron{
        triangle a,b,c,d,e,f,g,h;
        octahedron() :
        a(
          triangle(
                   (vert){1.0,0.0,0.0},
                   (vert){0.0,1.0,0.0},
                   (vert){0.0,0.0,1.0}
                   )
          ),
        b(
          triangle(
                   (vert){0.0,0.0,1.0},
                   (vert){0.0,1.0,0.0},
                   (vert){-1.0,0.0,0.0}
                   )
          ),
        c(
          triangle(
                   (vert){-1.0,0.0,0.0},
                   (vert){0.0,1.0,0.0},
                   (vert){0.0,0.0,-1.0}
                   )
          ),
        d(
          triangle(
                   (vert){0.0,0.0,-1.0},
                   (vert){0.0,1.0,0.0},
                   (vert){1.0,0.0,0.0}
                   )
          ),
        e(
          triangle(
                   (vert){0.0,-1.0,0.0},
                   (vert){1.0,0.0,0.0},
                   (vert){0.0,0.0,1.0}
                   )
          ),
        f(
          triangle(
                   (vert){0.0,-1.0,0.0},
                   (vert){0.0,0.0,1.0},
                   (vert){-1.0,0.0,0.0}
                   )
          ),
        g(
          triangle(
                   (vert){0.0,-1.0,0.0},
                   (vert){-1.0,0.0,0.0},
                   (vert){0.0,0.0,-1.0}
                   )
          ),
        h(
          triangle(
                   (vert){0.0,-1.0,0.0},
                   (vert){0.0,0.0,-1.0},
                   (vert){1.0,0.0,0.0}
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
            g.print();
            h.print();
        }
        void printInd(){
            a.printInd(0);
            b.printInd(1);
            c.printInd(2);
            d.printInd(3);
            e.printInd(4);
            f.printInd(5);
            g.printInd(6);
            h.printInd(7);
        }
        void subdiv(){
            a.subdiv();
            b.subdiv();
            c.subdiv();
            d.subdiv();
            e.subdiv();
            f.subdiv();
            g.subdiv();
            h.subdiv();
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
            g.save_v(file);
            std::cout << ".";
            h.save_v(file);
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
            std::cout << ".";
            g.save_i(file,6);
            std::cout << ".";
            h.save_i(file,7);
            std::cout << "." << std::endl;
        }
    };
    
public:
    octahedron data;
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

//
//  icosahedronsphere.h
//  icosahedron-sphere
//
//  Created by Grin on 01.03.15.
//  Copyright (c) 2015 FredMF. All rights reserved.
//

#ifndef icosahedron_sphere_icosahedronsphere_h
#define icosahedron_sphere_icosahedronsphere_h

#include <iostream>
#include <string>
#include <fstream>
#include <cmath>

class icosahedronsphere{
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
    
    struct icosahedron{
        triangle a,b,c,d,e,f,g,h,i,j,k,l,m,n,o,p,q,r,s,t;
        icosahedron() :
        a(
          triangle(
                   (vert){0.000000,-1.000000,0.000000},
                   (vert){0.723600,-0.447215,0.525720},
                   (vert){-0.276385,-0.447215,0.850640}
                   )
          ),
        b(
          triangle(
                   (vert){0.723600,-0.447215,0.525720},
                   (vert){0.000000,-1.000000,0.000000},
                   (vert){0.723600,-0.447215,-0.525720}
                   )
          ),
        c(
          triangle(
                   (vert){0.000000,-1.000000,0.000000},
                   (vert){-0.276385,-0.447215,0.850640},
                   (vert){-0.894425,-0.447215,0.000000}
                   )
          ),
        d(
          triangle(
                   (vert){0.000000,-1.000000,0.000000},
                   (vert){-0.894425,-0.447215,0.000000},
                   (vert){-0.276385,-0.447215,-0.850640}
                   )
          ),
        e(
          triangle(
                   (vert){0.000000,-1.000000,0.000000},
                   (vert){-0.276385,-0.447215,-0.850640},
                   (vert){0.723600,-0.447215,-0.525720}
                   )
          ),
        f(
          triangle(
                   (vert){0.723600,-0.447215,0.525720},
                   (vert){0.723600,-0.447215,-0.525720},
                   (vert){0.894425,0.447215,0.000000}
                   )
          ),
        g(
          triangle(
                   (vert){-0.276385,-0.447215,0.850640},
                   (vert){0.723600,-0.447215,0.525720},
                   (vert){0.276385,0.447215,0.850640}
                   )
          ),
        h(
          triangle(
                   (vert){-0.894425,-0.447215,0.000000},
                   (vert){-0.276385,-0.447215,0.850640},
                   (vert){-0.723600,0.447215,0.525720}
                   )
          ),
        i(
          triangle(
                   (vert){-0.276385,-0.447215,-0.850640},
                   (vert){-0.894425,-0.447215,0.000000},
                   (vert){-0.723600,0.447215,-0.525720}
                   )
          ),
        j(
          triangle(
                   (vert){0.723600,-0.447215,-0.525720},
                   (vert){-0.276385,-0.447215,-0.850640},
                   (vert){0.276385,0.447215,-0.850640}
                   )
          ),
        k(
          triangle(
                   (vert){0.723600,-0.447215,0.525720},
                   (vert){0.894425,0.447215,0.000000},
                   (vert){0.276385,0.447215,0.850640}
                   )
          ),
        l(
          triangle(
                   (vert){-0.276385,-0.447215,0.850640},
                   (vert){0.276385,0.447215,0.850640},
                   (vert){-0.723600,0.447215,0.525720}
                   )
          ),
        m(
          triangle(
                   (vert){-0.894425,-0.447215,0.000000},
                   (vert){-0.723600,0.447215,0.525720},
                   (vert){-0.723600,0.447215,-0.525720}
                   )
          ),
        n(
          triangle(
                   (vert){-0.276385,-0.447215,-0.850640},
                   (vert){-0.723600,0.447215,-0.525720},
                   (vert){0.276385,0.447215,-0.850640}
                   )
          ),
        o(
          triangle(
                   (vert){0.723600,-0.447215,-0.525720},
                   (vert){0.276385,0.447215,-0.850640},
                   (vert){0.894425,0.447215,0.000000}
                   )
          ),
        p(
          triangle(
                   (vert){0.276385,0.447215,0.850640},
                   (vert){0.894425,0.447215,0.000000},
                   (vert){0.000000,1.000000,0.000000}
                   )
          ),
        q(
          triangle(                  
                   (vert){-0.723600,0.447215,0.525720},
                   (vert){0.276385,0.447215,0.850640},
                   (vert){0.000000,1.000000,0.000000}
                   )
          ),
        r(
          triangle(                  
                   (vert){-0.723600,0.447215,-0.525720},
                   (vert){-0.723600,0.447215,0.525720},
                   (vert){0.000000,1.000000,0.000000}
                   )
          ),
        s(
          triangle(                  
                   (vert){0.276385,0.447215,-0.850640},
                   (vert){-0.723600,0.447215,-0.525720},
                   (vert){0.000000,1.000000,0.000000}
                   )
          ),
        t(
          triangle(                  
                   (vert){0.894425,0.447215,0.000000},
                   (vert){0.276385,0.447215,-0.850640},
                   (vert){0.000000,1.000000,0.000000}
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
            i.print();
            j.print();
            k.print();
            l.print();
            m.print();
            n.print();
            o.print();
            p.print();
            q.print();
            r.print();
            s.print();
            t.print();
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
            i.printInd(8);
            j.printInd(9);
            k.printInd(10);
            l.printInd(11);
            m.printInd(12);
            n.printInd(13);
            o.printInd(14);
            p.printInd(15);
            q.printInd(16);
            r.printInd(17);
            s.printInd(18);
            t.printInd(19);
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
            i.subdiv();
            j.subdiv();
            k.subdiv();
            l.subdiv();
            m.subdiv();
            n.subdiv();
            o.subdiv();
            p.subdiv();
            q.subdiv();
            r.subdiv();
            s.subdiv();
            t.subdiv();
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
            i.save_v(file);
            std::cout << ".";
            j.save_v(file);
            std::cout << ".";
            k.save_v(file);
            std::cout << ".";
            l.save_v(file);
            std::cout << ".";
            m.save_v(file);
            std::cout << ".";
            n.save_v(file);
            std::cout << ".";
            o.save_v(file);
            std::cout << ".";
            p.save_v(file);
            std::cout << ".";
            q.save_v(file);
            std::cout << ".";
            r.save_v(file);
            std::cout << ".";
            s.save_v(file);
            std::cout << ".";
            t.save_v(file);
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
            std::cout << ".";
            i.save_i(file,8);
            std::cout << ".";
            j.save_i(file,9);
            std::cout << ".";
            k.save_i(file,10);
            std::cout << ".";
            l.save_i(file,11);
            std::cout << ".";
            m.save_i(file,12);
            std::cout << ".";
            n.save_i(file,13);
            std::cout << ".";
            o.save_i(file,14);
            std::cout << ".";
            p.save_i(file,15);
            std::cout << ".";
            q.save_i(file,16);
            std::cout << ".";
            r.save_i(file,17);
            std::cout << ".";
            s.save_i(file,18);
            std::cout << ".";
            t.save_i(file,19);
            std::cout << "." << std::endl;
        }
    };
    
public:
    icosahedron data;
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

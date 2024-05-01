// TODO: Fix
#include "../types.hh"
#define SPEED 0.005
extern "C"{
    GameData* d;
    u8 W, S, A, D, space;
    u8 T, F, G, H;
    u8 getKey(const char* str){
        return (u64)d->getResource(4, (u64)str);
    }
    void Init(GameData* gd, u32 scriptIndex){
        GameObject go = emptyGameObj;
        go.script = scriptIndex;
        d = gd;
        d->gameObjects->push_back(go);
        d->cp->pos = {0, 0, 0};
    }
    void Start(u32 index){
        (void)index; // Unused
        W = getKey("w");
        A = getKey("a");
        S = getKey("s");
        D = getKey("d");
        space = getKey("space");
        T = getKey("T");
        F = getKey("F");
        G = getKey("G");
        H = getKey("H");
    }
    fx f = 0, wp = 0, ap = 0;
    void Update(u32 index){
        (void)index; // Unused
        if(d->keys[space]){
            f += 0.01;
            d->cp->rot = {0, f, 0};
            d->cp->sync(d->cp);
        }
        if(d->keys[W])
            wp -= SPEED;
        if(d->keys[S])
            wp += SPEED;
        if(d->keys[A])
            ap += SPEED;
        if(d->keys[D])
            ap -= SPEED;
        
        if(d->keys[T])
            ((*d->gameObjects)[3]).position.Z += 0.003;
        if(d->keys[G])
            ((*d->gameObjects)[3]).position.Z -= 0.003;
        if(d->keys[F])
            ((*d->gameObjects)[3]).position.X -= 0.003;
        if(d->keys[H])
            ((*d->gameObjects)[3]).position.X += 0.003;
        if(d->keys[H])
            d->screenShot("file.ppm");
        d->cp->pos = {ap, 0, wp};
    }
}

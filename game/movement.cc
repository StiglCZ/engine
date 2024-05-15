#include "../types.hh"
#include <unistd.h>
#define SPEED 0.3
extern "C"{
    GameData* d;
    u8 W, S, A, D, space, esc;
    u8 T, F, G, H;
    void Init(GameData* gd, u32 scriptIndex){
        GameObject go = emptyGameObj;
        go.script = scriptIndex;
        d = gd;
        d->gameObjects->push_back(go);
        d->cp->pos = {0, 0, 0};
    }
    void Start(u32 index){
        (void)index; // Unused
        W = getKey(d, "w");
        A = getKey(d, "a");
        S = getKey(d, "s");
        D = getKey(d, "d");
        space = getKey(d, "space");
        T = getKey(d, "T");
        F = getKey(d, "F");
        G = getKey(d, "G");
        H = getKey(d, "H");
        esc = getKey(d, "Escape");
    }
    fx f = 0, wp = 0, ap = 0;
    void Update(u32 index){
        (void)index; // Unused
        float trueDeltaTime = *d->deltaTime / 16700.0;
        if(d->keys[space]){
            f += 0.01;
            d->cp->rot = {0, f, 0};
            d->cp->sync(d->cp, false);
        }
        if(d->keys[W])
            wp -= SPEED * trueDeltaTime;
        if(d->keys[S])
            wp += SPEED * trueDeltaTime;
        if(d->keys[A])
            ap += SPEED * trueDeltaTime;
        if(d->keys[D])
            ap -= SPEED * trueDeltaTime;
        
        if(d->keys[T])
            ((*d->gameObjects)[3]).position.Z += 0.3 * trueDeltaTime;
        if(d->keys[G])
            ((*d->gameObjects)[3]).position.Z -= 0.3 * trueDeltaTime;
        if(d->keys[F])
            ((*d->gameObjects)[3]).position.X -= 0.3 * trueDeltaTime;
        if(d->keys[H])
            ((*d->gameObjects)[3]).position.X += 0.3 * trueDeltaTime;
        
        if(d->keys[esc]){
            d->drawText({(int)d->W / 2, (int)d->H / 2}, "PAUSED");
            while(d->keys[esc])
                usleep(1);
            while(!d->keys[esc])
                usleep(1);
            while(d->keys[esc])
                usleep(1);
        }
        d->cp->pos = {ap, 0, wp};
    }
}

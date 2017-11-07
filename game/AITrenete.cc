#include "Player.hh"


/**
 * Write the name of your player and save this file
 * with the same name and .cc extension.
 */
#define PLAYER_NAME Trenete


struct PLAYER_NAME : public Player {

  /**
   * Factory: returns a new instance of this class.
   * Do not modify this function.
   */
    static Player* factory () {
    return new PLAYER_NAME;
    }

    typedef vector<int> V;
    vector<V> mat; //matriu de distancies
    vector<Dir> desp;
    vector<V> assignades; //matriu de posicions assignades

inline bool movefarmer(Pos p) {
    CellType T = cell(p).type;
    int r = cell(p).id;
    if (T == Wall or r != -1 or cell(p).haunted) return false;
    return true;
}
inline bool movewitch(Pos p) {
    CellType T = cell(p).type;
    int r = cell(p).id;
    if (T == Wall or (r != -1 and unit(r).type == Witch) or p.i == 1 or p.i == 35 or p.j == 1 or p.j == 35
) return false;
    return true;
}
inline bool moveknight(Pos p) {
    CellType T = cell(p).type;
    //Unit u = unit(cell(p).id);
    if (T == Wall or cell(p).haunted) return false;
    return true;
}
inline bool no_enemies(Pos p) {  
    for (int i = p.i - 2; i <= p.i + 2; ++i) {
        for (int j = p.j - 2; j <= p.j + 2; ++j) {
            if (i >= 0 and i <= 36 and j >= 0 and j <= 36 and cell(i,j).id != -1) {
                Unit u = unit(cell(i,j).id);
                int dist = (p.i - i) + abs(p.j - j);
                if ((u.player != 0 and u.type == Knight)  
                    //or (u.type == Witch and dist < 3) 
                    or cell(i,j).haunted) 
                    return false;
            }
        }
    }
    return true;
}
inline bool segura(Pos p) {  
    for (int i = p.i - 2; i <= p.i + 2; ++i) {
        for (int j = p.j - 2; j <= p.j + 2; ++j) {
            if (i >= 0 and i <= 36 and j >= 0 and j <= 36 and cell(i,j).id != -1) {
                Unit u = unit(cell(i,j).id); 
                int dist = (p.i - i) + abs(p.j - j);
                if (u.type == Witch and dist < 3) return false;
            }
        }
    }
    return true;
}

Dir bfs_farmers(Pos p) { // bfs per farmers
    mat = vector<V>(rows(), V(cols(), -1));
    queue<Pos> fpos;
    bool found = false;  
    mat[p.i][p.j] = 0;   // inici distancia 0
    fpos.push(p);   // la posicio va a la cua
    while ((not fpos.empty()) and (not found)) {	  //mentre tinguem cua i no haguem trobat
	Pos p1 = fpos.front(); // p1 es la posicio del principi de la cua
	fpos.pop(); // Es treu									
	if (cell(p1).id == -1 and cell(p1).owner != 0 and no_enemies(p1)) {
	    found = true;
	    // retroces
	    Pos p2;
	    while (mat[p1.i][p1.j] != 0) {  //busca la posicio a la que estic
		for (int z = 0; z < 8; z=z+2) {
		    p2 = p1 + desp[z];
		    if (mat[p2.i][p2.j] == (mat[p1.i][p1.j]) - 1) p1 = p2; 
                    //si es troba una adjacent amb distancia menor, es la seguent
		    if (mat[p1.i][p1.j] == 0) {   //quan arribem al final retornem direccio
                        assignades[p2.i][p2.j] = 1;
			if (desp[z] == Right) return Left;
			if (desp[z] == Left) return Right;
			if (desp[z] == Top) return Bottom;
			if (desp[z] == Bottom) return Top;
		    }
		}
	    } // fi retroces
        }
	else {
	    for (int k = 0; k < 8; k=k+2) {
		if (movefarmer(p1 + desp[k])) {
		    Pos seg = p1 + desp[k]; //posicio seguent
		    if (pos_ok(seg) and mat[seg.i][seg.j] == -1 and assignades[seg.i][seg.j] == -1) {
			mat[seg.i][seg.j] = mat[p1.i][p1.j] + 1;
			fpos.push(seg);
		    }
		}
	    }
	}
    }// fi while
    for (int h = 0; h < 8; h=2+h) {
        Pos s = p + desp[h];
        if (movefarmer(s) and assignades[s.i][s.j] == -1) {
            assignades[s.i][s.j] == 1;
            return desp[h];
        }
    }
    return None; 
}
Dir bfs_knights(Pos p) { //bfs per knights
    mat = vector<V>(rows(), V(cols(), -1)); //matriu inicialitzada a 0
    queue<Pos> wpos;
    //int c = cell(p).id;
    //if (unit(c).health < 90 and segura(p)) return None;
    bool found = false;
    bool segona = true; 
    mat[p.i][p.j] = 0;   // inici distancia 0
    wpos.push(p);   // la posicio va a la cua
    while (not wpos.empty() and not found) { // mentre tinguem cua i no haguem trobat
        Pos p1 = wpos.front(); // p1 es la posicio del principi de la cua
        wpos.pop(); // Es treu
        int g = cell(p1).id;
        if (g != -1 and unit(g).type != Witch and unit(g).player != 0)  { 
            found = true;
            // retroces
            Pos p2;
            while (mat[p1.i][p1.j] != 0) {  //busca la posicio a la que estic
                for (int z = 0; z < 8; ++z) {
                    p2 = p1 + desp[z];
                    if (mat[p2.i][p2.j] == (mat[p1.i][p1.j]) - 1) p1 = p2;//si es troba una adjacent amb distancia menor es la seguent
                    if (mat[p1.i][p1.j] == 0) {   //quan arribem al final retornem
                        if (desp[z] == Right) return Left;
                        if (desp[z] == Left) return Right;
                        if (desp[z] == Top) return Bottom;
                        if (desp[z] == Bottom) return Top;
                        if (desp[z] == BR) return TL;
                        if (desp[z] == RT) return LB;
                        if (desp[z] == TL) return BR;
                        if (desp[z] == LB) return RT;
                    }        
                }
            } // fi retroces
        }
        else {
            for (int k = 0; k < 8; ++k) {
                if (moveknight(p1+desp[k])) {
                    Pos seg = p1+desp[k]; //posicio seguent
                    if (pos_ok(seg) and mat[seg.i][seg.j] == -1) {
                        if (not segona) {
                            mat[seg.i][seg.j] = mat[p1.i][p1.j] + 1;
                            wpos.push(seg);
                        }
                        else {
                            
                            if (segura(seg)) {
                                
                                mat[seg.i][seg.j] = mat[p1.i][p1.j] + 1;
                                wpos.push(seg);
                            }
                        }
                    }
                }       
            }
            segona = false;      
        }
    }
    for (int h = 0; h < 8; h=2+h) if (moveknight(p+desp[h])) return desp[h];
    return None; 
}
Dir bfs_witches(Pos p) { //bfs per bruixes
    mat = vector<V>(rows(), V(cols(), -1)); //matriu inicialitzada a 0
    queue<Pos> wpos;
    bool found = false;  
    mat[p.i][p.j] = 0;   // inici distancia 0
    wpos.push(p);   // la posicio va a la cua
    while (not wpos.empty() and not found) { // mentre tinguem cua i no haguem trobat
        Pos p1 = wpos.front(); // p1 es la posicio del principi de la cua
        wpos.pop(); // Es treu
        int g = cell(p1).id;
        if (g != -1 and (unit(g).type == Knight) and unit(g).player != 0 and segura(p1))  { 
            found = true;
            // retroces
            Pos p2;
            while (mat[p1.i][p1.j] != 0) {  //busca la posicio a la que estic
                for (int z = 0; z < 8; z=z+2) {
                    p2 = p1 + desp[z];
                    if (mat[p2.i][p2.j] == (mat[p1.i][p1.j]) - 1) p1 = p2;//si es troba una adjacent amb distancia menor es la seguent
                    if (mat[p1.i][p1.j] == 0) {   //quan arribem al final retornem
                        if (desp[z] == Right) return Left;
                        if (desp[z] == Left) return Right;
                        if (desp[z] == Top) return Bottom;
                        if (desp[z] == Bottom ) return Top;
                    }        
                }
            } // fi retroces
        }
        else {
            for (int k = 0; k < 8; k=k+2) {
                if (movewitch(p1+desp[k])) {
                    Pos seg = p1+desp[k]; //posicio seguent
                    if (pos_ok(seg) and mat[seg.i][seg.j] == -1) {
                        mat[seg.i][seg.j] = mat[p1.i][p1.j] + 1;
                        wpos.push(seg);
                    }
                }       
            }       
        }
    }
    for (int h = 0; h < 8; h=2+h) if (movewitch(p+desp[h])) return desp[h];
    return None; 
}

void play () {
    mat = vector<V>(rows(), V(cols(), -1)); //matriu
    assignades = vector<V>(rows(), V(cols(), -1));
    if (round() == 0) {
        desp = vector<Dir>(8); //vector de direccions
        desp[0] = Bottom; desp[1] = BR; desp[2] = Right; desp[3] = RT;
        desp[4] = Top; desp[5] = TL; desp[6] = Left; desp[7] = LB; 
    }
    V w = witches(0);
    for (int id : w) {
	Pos pos = unit(id).pos;
	Dir dw = bfs_witches(pos);
	command(id, dw);
    }
    V f = farmers(0);
    for (int id : f) {
        Pos pos = unit(id).pos;
        Dir df = bfs_farmers(pos);
        command(id, df);
    }
    V k = knights(0);
    for (int id : k) {
	Pos pos = unit(id).pos;
	Dir dk = bfs_knights(pos);
	command(id, dk);
    }
}
};

/**
 * Do not modify the following line.
 */
RegisterPlayer(PLAYER_NAME);

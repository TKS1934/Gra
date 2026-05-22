using namespace std;

vector<vector<int>> generowanie_pol(int x,int y){
    int i,j;
    vector<vector<int>> t;
    srand(time(NULL));
    for(i=0;i<y;i++){
        vector<int> pom;
        for(j=0;j<x;j++){
            int z=rand()%4;
            pom.push_back(z);
        }
        t.push_back(pom);
    }
    return t;

}
vector<vector<blok>> generowanie_planszy(int x,int y, const sf::RenderWindow& window,float graw){
    vector<vector<int>> t=generowanie_pol(x,y);
    sf::Vector2f size1={float(window.getSize().x)/x,float(window.getSize().y)/y};
    vector<vector<blok>> tk;
    int i=0,j=0;
    for(auto w:t){
        vector<blok> pom;
        for(auto z:w){
            blok pom1(size1,{float(i*size1.x),float(j*size1.y)},{0,0},window,graw,z);
            pom.push_back(pom1);
            i++;
        }
        tk.push_back(pom);
        j++;
        i=0;
    }
    return tk;
    //blok *obj;
    // obj = new blok(size1,{200,550},{0,-10},window,grawitacja,1);
    //(const sf::Vector2f& size,const sf::Vector2f& position, const sf::Vector2f& velocity, const sf::RenderWindow& window,float grawitacja, int ID)
}


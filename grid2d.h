#ifndef Grid2d_h
#define Grid2d_h

#ifndef nullptr
#define nullptr NULL
#endif

#include <vector>

template <typename T>
class Grid2d {
    public:
        Grid2d(int width, int height, const T& default_val, const T& null_val);
        //void destroy(); //WARNING: only use this with T*!!!!

        bool inBounds(int x, int y);
        
        T    getAt(int x, int y);
        void setAt(int x, int y, const T& val);

        T resetTo(T reset);

        std::vector<T> getSurroundings(int x, int y);
        T getNeighbor(int direction, int x, int y);

        void fill(int x, int y, const T& old_t, const T& new_t);

        int getWidth();
        int getHeight();
        int getSize();

    protected:
        int width, height;
        T default_val, null_val;
        std::vector<std::vector<T> > layout;
};

template <typename T>
Grid2d<T>::Grid2d(int width, int height, const T& default_val, const T& null_val) :
    width(width), height(height), default_val(default_val), null_val(null_val)
{
    for (int y = 0; y < height; y++){
        std::vector<T> current_line;
        for (int x = 0; x < width; x++){
            current_line.push_back(default_val);
        }
        
        layout.push_back(current_line);
    }
}

template <typename T>
bool Grid2d<T>::inBounds(int x, int y){
    //throw error if not
    return !(x > width-1 || y > height-1 || x < 0 || y < 0);
}

template <typename T>
T Grid2d<T>::getAt(int x, int y){
    if (!inBounds(x, y)){
        return null_val;
    } else {
        return layout[y][x];
    }
}

template <typename T>
void Grid2d<T>::setAt(int x, int y, const T& val){
    if (!inBounds(x, y)){
        return;
    } else {
        layout[y][x] = val;
    }
}

template <typename T>
T Grid2d<T>::resetTo(T reset){
    for (int y = 0; y < height; y++){
        std::vector<T> current_line;
        for (int x = 0; x < width; x++){
            current_line.push_back(reset);
        }
        
        layout.push_back(current_line);
    }
}

template <typename T>
std::vector<T> Grid2d<T>::getSurroundings(int x, int y){
    std::vector<T> out;
    for (int j = -1; j < 2; j++){
        for (int i = -1; i < 2; i++){
            if (x + i < 0 || x + i > width || y + j < 0 || y + j > height){
                continue;
            } else {
                out.push_back(getAt(x+i, y+j));
            }
        }
    }
}

template <typename T>
T Grid2d<T>::getNeighbor(int direction, int x, int y){
    switch (direction){
        case 7: //northwest
            return getAt(x-1, y-1);
        case 8: //north
            return getAt(x, y-1);
        case 9: //northeast
            return getAt(x+1, y-1);
        case 4: //west
            return getAt(x-1, y);
        case 6: //east
            return getAt(x+1, y);
        case 1: //southwest
            return getAt(x-1, y+1);
        case 2: //south
            return getAt(x, y+1);
        case 3: //southeast
            return getAt(x+1, y+1);
        default:
            //throw an error: invalid direction
            return null_val;
    }
}

template <typename T>
void Grid2d<T>::fill(int x, int y, const T& old_t, const T& new_t){
    //textbook fill algorithm
    if (!inBounds(x, y)){
        return;
    }

  //north
    if (getAt(x, y-1) == old_t){
        setAt(x, y-1, new_t);
        fill(x, y-1, old_t, new_t);
    }      
  //east
    if (getAt(x+1, y) == old_t){
        setAt(x+1, y, new_t);
        fill(x+1, y, old_t, new_t);
    }     
  //south{
    if (getAt(x, y+1) == old_t){
        setAt(x, y+1, new_t);
        fill(x, y+1, old_t, new_t);
    }      
  //west
    if (getAt(x-1, y) == old_t){
        setAt(x-1, y, new_t);
        fill(x-1, y, old_t, new_t);
    }
}

template <typename T> int Grid2d<T>::getWidth (){return width;}
template <typename T> int Grid2d<T>::getHeight(){return height;}
template <typename T> int Grid2d<T>::getSize  (){return width*height;}

#endif

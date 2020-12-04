#include <iostream>
#include <vector>
#include <tuple>
#include <stdexcept>
#include <sstream>

using namespace std;
using xtype = int16_t;

int main()
{
    const size_t max_mod{1000}; //модуль максимального значения в матрице

    //получаем размер матрицы
    cin.exceptions(std::ios::failbit);
    size_t tmp_m{0};
    //cout << "введите нечетный размер матрицы: " << endl;
    cin >> tmp_m;

    if( tmp_m%2 != 1 )
        throw std::invalid_argument("размер матрицы должен быть нечетным");

    const size_t m{tmp_m};

    //получаем значения элементов матрицы
    std::vector<xtype> cont(m*m); //матрица будет представлена вектором
    for(size_t i=0; i<m; ++i)
    {
        //cout << "введите строку матрицы" << endl;
        for(size_t j=0; j<m; ++j)
        {
            xtype x{0};
            cin >> x;
            if( std::abs(x) > max_mod )
                throw std::invalid_argument("некорректное значение элемента матрицы");

            cont[(i*m)+j] = x;
        }
    }

    //получаем индекс центра матрицы
    const size_t center = m/2;

    //вспомогательные функции
    //xarr - получаем значения из вектора, как из двумерного массива
    auto get_val = [&cont, m](size_t x, size_t y){
        return cont.at( (y*m) + x );
    };

    enum class Direction
    {
        Unknown,
        Up,
        Right,
        Down,
        Left
    };

    using xpoint = std::tuple<size_t, size_t, Direction>;
        //позиция x, позиция y, текущее направление раскручивания спирали

    //возвращает точку, слудующую за переданной
    //будем раскручиваться, пока не достигнем точки (0,0)
    auto move_to_next = [center](xpoint &point) -> void
    {
        auto &[x, y, dir] = point;
        ssize_t dx = center-x;
        ssize_t dy = center-y;

        //если модуль dx равен модулю dy, то мы на точке поворота
        //  если (dx,dy):
        //      находимся в центре (0,0), поднимаемся на виток, потом идем направо. --y
        //      находимся на 10:30. (>0,>0), поднимаемся на виток, потом идем направо. --y
        //      находимся на 01:30 (<0,>0), то идем вниз. ++y
        //      находимся на 04:30 (<0,<0), то идем налево. --x
        //      находимся на 07:30 (>0,<0), то идем вверх. --y
        //и всё это до тех пор, пока не получим точку 0,0...

        if( std::abs(dx) == std::abs(dy) )
        {
            if( (dx>0 && dy>0) || (dx==0 && dy==0) ) //10:30 или центр
            {
                --y;
                dir = Direction::Right;
            }
            else if( dx<0 && dy>0 ) //01:30
            {
                ++y;
                dir = Direction::Down;
            }
            else if( dx<0 && dy<0 ) // 04:30
            {
                --x;
                dir = Direction::Left;
            }
            else if( dx>0 && dy<0 ) // 07:30
            {
                --y;
                dir = Direction::Up;
            }
            else
                throw std::logic_error("нарушена логика поворотных точек");
        }
        else
        {
            //мы не на поворотной точке, продолжаем движение в прежнем направлении
            if( dir == Direction::Up )
                --y;
            else if( dir == Direction::Right )
                ++x;
            else if( dir == Direction::Down )
                ++y;
            else if( dir == Direction::Left )
                --x;
            else
                throw std::logic_error("нарушена логика прямых направлений");
        }
    };

    //раскручиваем спираль

    cout << get_val(center,center) << endl; //выводим центр матрицы
    if( center == 0 ) //и обрабатываем матрицу размера 1
        return 0;

    xpoint point{center, center, Direction::Unknown}; //позиционируемся в центр
    std::stringstream ss;   //постоянный вывод в cout очень затратен по времени
                            //используем промежуточный буфер для накопления данных
                            //при привышении предела по памяти можно выводить и очищать
                            //буфер, например, после 1000 итераций
    while(true)
    {
        move_to_next(point);
        ss << get_val(std::get<0>(point),std::get<1>(point)) << "\n";
        if( std::get<0>(point) == 0 && std::get<1>(point) == 0 )
            break;
    }

    cout << ss.str();

    return 0;
}

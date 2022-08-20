#include "MapViewerWind.h"
#include "ui_MapViewerWind.h"

#include "processMapFiles.h"

#include <QFileDialog>
#include <QMessageBox>

#include <QPainter>
#include <QPen>
#include <QFont>

#include <list>
#include <mutex>

static const int current_max_base_color=61;

std::array<ARGB,256> make_map_LUT();
std::array<ARGB,256> make_inverse_map_LUT(const std::array<ARGB,256> &);

const std::array<ARGB,256> map_color_to_ARGB=make_map_LUT();
const std::array<ARGB,256> inverse_map_color_to_ARGB
    =make_inverse_map_LUT(map_color_to_ARGB);

std::array<ARGB,256> make_map_LUT() {
    std::array<ARGB,256> result;

    result.fill(0x7FFF0000);

    const Eigen::Map<const Eigen::Array<float,256,3>> src(SlopeCraft::RGBBasicSource);

    for(int row_idx=0;row_idx<256;row_idx++) {
        ARGB a,r,g,b;
        const int base_color=row_idx%64;
        const int shade=row_idx/64;
        const int map_color=base_color*4+shade;

        if(base_color>current_max_base_color) {
            continue;
        }
        r=std::min(255U,uint32_t(255*src(row_idx,0)));
        g=std::min(255U,uint32_t(255*src(row_idx,1)));
        b=std::min(255U,uint32_t(255*src(row_idx,2)));

        a=(base_color!=0)?(255):(0);

        result[map_color]=(a<<24)|(r<<16)|(g<<8)|(b);
    }

    return result;
}

std::array<ARGB,256> make_inverse_map_LUT(const std::array<ARGB,256> & src) {
    std::array<ARGB,256> result;
    for(size_t idx=0;idx<src.size();idx++) {
        const ARGB argb=src[idx];
        ARGB r=(argb>>16)&0xFF;
        ARGB g=(argb>>8)&0xFF;
        ARGB b=(argb)&0xFF;

        r=255-r;
        g=255-g;
        b=255-b;
        result[idx]=(0xFF<<24)|(r<<16)|(g<<8)|(b);
    }
    return result;
}

MapViewerWind::MapViewerWind(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MapViewerWind)
{
    ui->setupUi(this);

    connect(ui->spinbox_rows,&QSpinBox::valueChanged,
            this,&MapViewerWind::update_contents);
    connect(ui->spinbox_cols,&QSpinBox::valueChanged,
            this,&MapViewerWind::update_contents);
    connect(ui->radio_is_row_major,&QRadioButton::clicked,
            this,&MapViewerWind::update_contents);
    connect(ui->radio_is_col_major,&QRadioButton::clicked,
            this,&MapViewerWind::update_contents);

    connect(ui->botton_clear,&QPushButton::clicked,
            this,&MapViewerWind::clear_all);

    connect(ui->combobox_select_map,&QComboBox::currentIndexChanged,
            this,&MapViewerWind::render_single_image);
    connect(ui->spinbox_pixel_size,&QSpinBox::valueChanged,
            this,&MapViewerWind::render_single_image);
    connect(ui->radio_show_base_color,&QRadioButton::clicked,
            this,&MapViewerWind::render_single_image);
    connect(ui->radio_show_color_only,&QRadioButton::clicked,
            this,&MapViewerWind::render_single_image);
    connect(ui->radio_show_map_color,&QRadioButton::clicked,
            this,&MapViewerWind::render_single_image);
    connect(ui->radio_show_shade,&QRadioButton::clicked,
            this,&MapViewerWind::render_single_image);
    connect(ui->checkbox_single_map_show_grid,&QCheckBox::clicked,
            this,&MapViewerWind::render_single_image);

    connect(ui->slider_resize_image,&QSlider::valueChanged,
            this,&MapViewerWind::render_composed);
}

MapViewerWind::~MapViewerWind()
{
    for(QLabel * label : this->labels) {
        if(label!=nullptr)
            delete label;
    }
    delete ui;
}

void MapViewerWind::update_contents() {
    this->reshape_tables();
    ui->label_show_map_count->setText(tr("地图数：")+QString::number(this->maps.size()));

    //update combo box
    ui->combobox_select_map->clear();
    for(auto & map : this->maps) {
        ui->combobox_select_map->addItem(map.filename);
    }

    this->render_composed();
}

void MapViewerWind::reshape_tables() {
    const int rows=ui->spinbox_rows->value();
    const int cols=ui->spinbox_cols->value();
    const bool is_col_major=ui->radio_is_col_major->isChecked();

    this->clear_all();

    ui->table_display_filename->setRowCount(rows);
    ui->table_display_filename->setColumnCount(cols);

    for(int idx=0;idx<int(this->maps.size());idx++) {
        const int r=(is_col_major)?(idx%rows):(idx/cols);
        const int c=(is_col_major)?(idx/rows):(idx%cols);
        if(r>=rows||c>=cols)
            continue;

        QTableWidgetItem * item=new QTableWidgetItem(this->maps[idx].filename);

        ui->table_display_filename->setItem(r,c,item);
    }

    for(QLabel * label : this->labels) {//detach label and layout
        if(ui->grid_layout_compose_maps->indexOf(label)>=0) {
            ui->grid_layout_compose_maps->removeWidget(label);
        }
        label->hide();
    }

    while(this->labels.size()<this->maps.size()) {
        this->labels.emplace_back(new QLabel(this));
        this->labels.back()->hide();
        this->labels.back()->update();
    }

    for(int idx=0;idx<int(this->maps.size());idx++) {
        const int r=(is_col_major)?(idx%rows):(idx/cols);
        const int c=(is_col_major)?(idx/rows):(idx%cols);
        if(r>=rows||c>=cols)
            continue;
        ui->grid_layout_compose_maps->addWidget(this->labels[idx],r,c);
    }

}

void MapViewerWind::clear_all() {
    ui->table_display_filename->clearContents();
    ui->combobox_select_map->clear();
    ui->label_show_single_map->setPixmap(QPixmap());
    ui->label_show_map_count->setText(tr("请选择地图文件"));

    for(QLabel * label : this->labels) {
        if(ui->grid_layout_compose_maps->indexOf(label)>=0) {
            ui->grid_layout_compose_maps->removeWidget(label);
        }
        label->hide();
    }
}


void MapViewerWind::render_single_image() {
    //ui->label_show_single_map->setPixmap(QPixmap());

    static int prev_pixel_size=-1;
    static int prev_idx=-1;
    static uint8_t prev_show_grid=2;

    //static single_map_draw_type prev_draw_type=color_only;

    const int current_idx=ui->combobox_select_map->currentIndex();
    if(current_idx<0 || current_idx>=int(this->maps.size())) {
        ui->label_show_single_map->setPixmap(QPixmap());
        prev_idx=-1;
        return;
    }

    const int pixel_size=ui->spinbox_pixel_size->value();
    const int rows=pixel_size*128,cols=pixel_size*128;
    const uint8_t show_grid=ui->checkbox_single_map_show_grid->isChecked();

    const bool is_color_only_image_changed=
            (pixel_size!=prev_pixel_size)||(prev_idx!=current_idx)||(show_grid!=prev_show_grid);
    // update previous
    prev_pixel_size=pixel_size;
    prev_idx=current_idx;
    prev_show_grid=show_grid;

    static QImage new_image;

    if(is_color_only_image_changed) {
        //cout<<"repaint"<<endl;
    }
    else {
        //cout<<"Don't repaint"<<endl;
    }

    if(is_color_only_image_changed) { // if color only image is changed, repaint it
        new_image=QImage(cols,rows,QImage::Format_ARGB32);
        new_image.fill(QColor(255,255,255,255));

        Eigen::Map<Eigen::Array<ARGB,Eigen::Dynamic,Eigen::Dynamic,Eigen::RowMajor>>
                map_new_image(reinterpret_cast<ARGB*>(new_image.scanLine(0)),rows,cols);

        const Eigen::Map<const Eigen::Array<ARGB,128,128,Eigen::RowMajor>>
                image_map(reinterpret_cast<ARGB*>(this->maps[current_idx].image.scanLine(0)));

        for(int c=0;c<image_map.cols();c++) {
            for(int r=0;r<image_map.rows();r++) {
                map_new_image.block(pixel_size*r,pixel_size*c,pixel_size,pixel_size)
                        .fill(image_map(r,c));
            }
        }

        if(pixel_size>=4&&show_grid) {
            for(int map_r_idx=0;map_r_idx<128;map_r_idx++) {
                for(int img_c_idx=0;img_c_idx<cols;img_c_idx++) {
                    const uint8_t map_color=
                            this->maps[current_idx].content()(map_r_idx,img_c_idx/pixel_size);
                    map_new_image(map_r_idx*pixel_size,img_c_idx)=
                            ::inverse_map_color_to_ARGB[map_color];
                }
            }


            for(int map_c_idx=0;map_c_idx<128;map_c_idx++) {
                for(int img_r_idx=0;img_r_idx<rows;img_r_idx++) {
                    const uint8_t map_color=
                            this->maps[current_idx].content()(img_r_idx/pixel_size,map_c_idx);
                    map_new_image(img_r_idx,map_c_idx*pixel_size)=
                            ::inverse_map_color_to_ARGB[map_color];
                }
            }
        }
    }

    ui->label_show_single_map->setPixmap(QPixmap::fromImage(new_image));

    // determine the value of draw_type
    single_map_draw_type draw_type=color_only;
    {
        if(ui->radio_show_base_color->isChecked())
            (draw_type)=base_color;
        if(ui->radio_show_map_color->isChecked())
            (draw_type)=map_color;
        if(ui->radio_show_shade->isChecked())
            (draw_type)=shade;
    }

    //cout<<"draw type = "<<draw_type<<endl;

    const int number_digits=int(draw_type);

    if(draw_type==color_only) {
        return;
    }



    const int avaliable_pixels_by_height=(pixel_size-6);

    const int avaliable_pixels_by_width=(number_digits==0)?(-1):
            ((pixel_size-4)/number_digits*2);

    const int avaliable_pixels=std::min(avaliable_pixels_by_height,avaliable_pixels_by_width);

    static constexpr int minimum_text_height=8;

    if(avaliable_pixels<minimum_text_height) {
        return;
    }



    std::unique_ptr<u8Array128RowMajor>
            displayed_numbers(new u8Array128RowMajor);


    switch (draw_type) {
    case map_color:
        //*displayed_numbers=this->maps[current_idx].content();
        memcpy(displayed_numbers->data(),this->maps[current_idx].content().data(),
               this->maps[current_idx].content().size());
        break;
    case base_color:
        *displayed_numbers=(this->maps[current_idx].content())/4;
        break;
    case shade:
        memcpy(displayed_numbers->data(),this->maps[current_idx].content().data(),
               this->maps[current_idx].content().size());

        constexpr uint64_t mask=0b0000001100000011000000110000001100000011000000110000001100000011;

        for(uint32_t idx=0;
            idx<displayed_numbers->size()*sizeof(uint8_t)/sizeof(uint64_t);
            idx++) {
            uint64_t & val=*(idx+reinterpret_cast<uint64_t*>(displayed_numbers->data()));
            val=val&mask;
        }
        /*
        for(int64_t idx=0;idx<displayed_numbers->size();idx++) {
            displayed_numbers->operator()(idx)
                    =displayed_numbers->operator()(idx)%4;
        }
        */
        break;
    }



    QPixmap temp_image=QPixmap::fromImage(new_image);

    // begin to draw numbers
    QPainter painter(&temp_image);
    if(!painter.isActive()) {
        cout<<"not active"<<endl;
        return;
    }
    painter.setRenderHint(QPainter::RenderHint::Antialiasing,true);

    {
        QFont font;
        font.setPixelSize(avaliable_pixels);
        painter.setFont(font);
    }


    QString text;
    text.resize(number_digits,'0');


    for(int r=0;r<128;r++) {
        for(int c=0;c<128;c++) {
            const ARGB invcolor=
                    ::inverse_map_color_to_ARGB[this->maps[current_idx].content()(r,c)];
            painter.setPen(QColor(invcolor));
            QRect rect(c*pixel_size,r*pixel_size,pixel_size,pixel_size);
            uint8_t cur_number=(*displayed_numbers)(r,c);

            switch (draw_type) {
                case map_color:
                    text[0]=char((cur_number/100)+'0');
                    cur_number-=(cur_number/100)*100;
                    text[1]=char((cur_number/10)+'0');
                    text[2]=char((cur_number%10)+'0');
                break;
            case base_color:
                    text[0]=char(cur_number/10+'0');
                    text[1]=char(cur_number%10+'0');
                break;
            default:
                    text[0]=char((cur_number&0b11)+'0');
                            break;
            }

            painter.drawText(rect,Qt::AlignVCenter|Qt::AlignHCenter,text);
        }
    }


    ui->label_show_single_map->setPixmap(temp_image);
}

void MapViewerWind::render_composed() {
    const int scale=ui->slider_resize_image->value();
    ui->label_show_compose_scaling->setText(QStringLiteral("×")+QString::number(scale));

    const int rows=ui->spinbox_rows->value();
    const int cols=ui->spinbox_cols->value();
    const bool is_col_major=ui->radio_is_col_major->isChecked();

    for(int idx=0;idx<int(this->maps.size());idx++) {
        const int r=(is_col_major)?(idx%rows):(idx/cols);
        const int c=(is_col_major)?(idx/rows):(idx%cols);
        if(scale<=1) {
            this->labels[idx]->setPixmap(QPixmap::fromImage(this->maps[idx].image));
        }
        else {
            this->labels[idx]->setPixmap(QPixmap::fromImage(
                                            (this->maps[idx].image.scaled(128*scale,128*scale)
                                            )));
        }

        if(r>=rows||c>=cols) {
            this->labels[idx]->hide();
        }
        else {
            this->labels[idx]->show();
        }

    }
}

void MapViewerWind::on_button_load_maps_clicked() {

    QStringList filenames=
            QFileDialog::getOpenFileNames(this,tr("选择地图数据文件"),"","map_*.dat");
    if(filenames.size()<=0) {
        return;
    }

    this->clear_all();
    this->maps.clear();
    this->maps.resize(filenames.size());

    std::list<std::pair<QString,std::string>> error_list;
    std::mutex lock;

#pragma omp parallel for
    for(int idx=0;idx<filenames.size();idx++) {

        std::string error_info;
        if(!process_map_file(filenames[idx].toLocal8Bit().data(),
                             (this->maps[idx].map_content).get(),
                             &error_info)) {
            lock.lock();

            error_list.emplace_back(filenames[idx],error_info);

            this->maps[idx].filename="";

            lock.unlock();
        }
        else {
            const int last_idx_of_reverse_slash=filenames[idx].lastIndexOf('\\');
            const int last_idx_of_slash=filenames[idx].lastIndexOf('/');
            const int last_idx_of_seperator=std::max(last_idx_of_reverse_slash,last_idx_of_slash);

            const int basename_length=filenames[idx].length()-last_idx_of_seperator-1;

            this->maps[idx].filename=filenames[idx].last(basename_length);
        }
    }

    for(const auto & error : error_list) {
        QMessageBox::StandardButton user_clicked=
        QMessageBox::warning(this,tr("加载地图文件失败"),
                                 tr("出错的文件：")+error.first+"\n错误信息：\n"
                                 +QString::fromLocal8Bit(error.second.data()),
                             {QMessageBox::StandardButton::Ignore,QMessageBox::StandardButton::NoToAll},
                             QMessageBox::StandardButton::Ignore
                             );
        if(user_clicked==QMessageBox::StandardButton::Ignore) {
            continue;
        }
        else {
            break;
        }
    }

    for(auto it=this->maps.begin();it!=this->maps.end();) {
        if(it->filename.isEmpty()) {
            it=this->maps.erase(it);
        }
        else {
            ++it;
        }
    }

#pragma omp parallel for
    for(map & map : this->maps) {
        map.image=QImage(128,128,QImage::Format::Format_ARGB32);
        Eigen::Map<Eigen::Array<ARGB,128,128,Eigen::RowMajor>>
                image_map(reinterpret_cast<ARGB*>(map.image.scanLine(0)));
        for(int64_t idx=0;idx<map.map_content->size();idx++) {
            image_map(idx)=map_color_to_ARGB[map.map_content->operator()(idx)];
        }
    }


    update_contents();

}

void MapViewerWind::on_checkbox_composed_show_spacing_toggled(bool is_checked) {
    const int spacing=(is_checked)?(6):(0);
    ui->grid_layout_compose_maps->setHorizontalSpacing(spacing);
    ui->grid_layout_compose_maps->setVerticalSpacing(spacing);
}


void MapViewerWind::on_button_save_single_clicked() {
    if(this->maps.size()<=0)
        return;

    if(ui->label_show_single_map->pixmap().isNull()) {
        return;
    }

    QPixmap pixmap=ui->label_show_single_map->pixmap();

    const QString dest=
            QFileDialog::getSaveFileName(this,tr("保存为图片"),"","*.png;;*.jpg;;*.gif");

    if(dest.isEmpty()) {
        return;
    }

    pixmap.save(dest);
}


void MapViewerWind::on_button_save_composed_clicked() {
    if(this->maps.size()<=0)
        return;

    const QString dest=
            QFileDialog::getSaveFileName(this,tr("保存为图片"),"","*.png;;*.jpg;;*.gif");
    if(dest.isEmpty())
        return;

    const int map_rows=ui->spinbox_rows->value();
    const int map_cols=ui->spinbox_cols->value();
    const bool is_col_major=ui->radio_is_col_major->isChecked();

    const int pixel_rows=128*map_rows;
    const int pixel_cols=128*map_cols;

    QImage result(pixel_cols,pixel_rows,QImage::Format_ARGB32);

    if(result.isNull()) {
        cout<<"Failed to export image."<<endl;
        return;
    }

    Eigen::Map<Eigen::Array<ARGB,Eigen::Dynamic,Eigen::Dynamic,Eigen::RowMajor>>
            emap_result(reinterpret_cast<ARGB*>(result.scanLine(0)),
                        pixel_rows,pixel_cols);

    emap_result.fill(0);

    for(int map_idx=0;map_idx<int(this->maps.size());map_idx++) {
        const int map_r=(is_col_major)?(map_idx%map_rows):(map_idx/map_cols);
        const int map_c=(is_col_major)?(map_idx/map_rows):(map_idx%map_cols);

        if(map_r>=map_rows||map_c>=map_cols) {
            continue;
        }

        Eigen::Map<const u32Array128RowMajor>
                emap_qimg(reinterpret_cast<ARGB*>(this->maps[map_idx].image.scanLine(0)));
        emap_result.block<128,128>(128*map_r,128*map_c)=emap_qimg;
    }

    result.save(dest);

}


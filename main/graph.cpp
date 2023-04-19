#include "graph.h"
#include "utils.h"

position point_to_position(graph graph, uint16_t x, uint16_t y) {
  uint16_t x_axis_length = graph.width - 2 * graph.padding;
  float x_interval = graph.x_max - graph.x_min;
  uint16_t y_axis_length = graph.height - 2 * graph.padding;
  float y_interval = graph.y_max - graph.y_min;

  position origin = {graph.pos.x + graph.padding, graph.pos.y + graph.height - graph.padding};
  position point = {origin.x + x / x_interval * x_axis_length, origin.y - y / y_interval * y_axis_length};

  return point;
}

void add_point(graph graph, uint16_t x, uint16_t y, char marker) {
  position point = point_to_position(graph, x, y);

  switch(marker) {
    case 'x':
      display.Draw_Line(point.x - 2, point.y - 2, point.x + 2, point.y + 2);
      display.Draw_Line(point.x - 2, point.y + 2, point.x + 2, point.y - 2);
    break;
    case '+':
      display.Draw_Fast_HLine(point.x - 2, point.y, 5);
      display.Draw_Fast_VLine(point.x, point.y - 2, 5);
    break;
    case 'o':
      display.Draw_Pixel(point.x - 1, point.y);
      display.Draw_Pixel(point.x + 1, point.y);
      display.Draw_Pixel(point.x, point.y - 1);
      display.Draw_Pixel(point.x, point.y + 1);
    break;
    case '.':
      display.Draw_Pixel(point.x, point.y);
    break;
    case 'O':
      display.Draw_Fast_VLine(point.x - 2, point.y - 1, 3);
      display.Draw_Fast_HLine(point.x - 1, point.y - 2, 3);
      display.Draw_Fast_VLine(point.x + 2, point.y - 1, 3);
      display.Draw_Fast_HLine(point.x - 1, point.y + 2, 3);
    break;
  }
}

void add_y_mark(graph graph, uint16_t y) {
  uint8_t original_rotation = display.Get_Rotation();
  
  // calculations are made using coordinates without rotation
  position mark_pos = point_to_position(graph, 0, y);
  mark_pos.y = display.Get_Height() - mark_pos.y - 1;
  uint8_t text_size = graph.padding / 8;
  uint16_t text_offset = 6 * (count_decimals(y) / 2) * text_size - 2;

  // x and y are switched because of the rotation
  display.Set_Rotation((original_rotation - 1) % 4);
  display.Draw_Fast_VLine(mark_pos.y, mark_pos.x - 2, 5);
  display.Print_Number_Int(y, mark_pos.y - text_offset, mark_pos.x - text_size * 8, 1, ' ', 10);
  display.Set_Rotation(original_rotation);
}

void add_x_mark(graph graph, uint16_t x) {
  position mark_pos = point_to_position(graph, x, 0);
  uint8_t text_size = graph.padding / 8;
  uint16_t text_offset = 6 * (count_decimals(x) / 2) * text_size - 2;

  display.Draw_Fast_VLine(mark_pos.x, mark_pos.y - 2, 5);
  display.Print_Number_Int(x, mark_pos.x - text_offset, mark_pos.y + 2, 1, ' ', 10);
}

void add_line(graph graph, float x1, float y1, float x2, float y2) {
  position point_1 = point_to_position(graph, x1, y1);
  position point_2 = point_to_position(graph, x2, y2);

  display.Draw_Line(point_1.x, point_1.y, point_2.x, point_2.y);
}

void add_pattern_line(graph graph, float x1, float y1, float x2, float y2, char pattern) {
  position point_1 = point_to_position(graph, x1, y1);
  position point_2 = point_to_position(graph, x2, y2);

  draw_pattern_line(point_1.x, point_1.y, point_2.x, point_2.y, pattern);
}

void draw_graph(graph graph) {
  display.Draw_Fast_HLine(graph.pos.x, graph.pos.y + graph.height - graph.padding, graph.width); // x axis
  display.Draw_Fast_VLine(graph.pos.x + graph.padding, graph.pos.y, graph.height); // y axis

  uint8_t arrow_size = 5;

  draw_arrow({graph.pos.x + graph.width, graph.pos.y + graph.height - graph.padding}, 1, graph.padding, graph.padding/2); // x axis arrow
  draw_arrow({graph.pos.x + graph.padding, graph.pos.y}, 0, graph.padding, graph.padding/2); // y axis arrow
}
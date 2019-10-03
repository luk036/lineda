#include <kd_tree.hpp>
#include <point.hpp>
#include <svgstream.hpp>
#include <vector>

using namespace std;

int main() {
  size_t i;

  /**Construct random points */
  vector<point> sinks(100);
  for (i = 0; i < sinks.size(); ++i) {
    sinks[i] = point::random();
  }

  kd_tree<point> tree;
  tree.create(sinks);
  svgstream sout("kdtree.svg");
  sout << tree;
}
x

    // point b[]= {point(2,10), point(8,9), point(528,35), point(43,89),
    // point(752,2308), point(1,3),
    // point(85,22),point(9324,750),point(849,2000),point(50,50),point(100,300)};
    point a[] = {point(10, 800), point(20, 600), point(30, 400),
                 point(40, 200), point(50, 100), point(120, 320),
                 point(210, 60), point(560, 30)};
sink_set sinks(8);
for (i = 0; i < sinks.size(); ++i) {
  sinks[i] = a[i];
}

// int x1 = 10;
// int y1 = 130;
// int x2 = 500;
// int y2 = 900;
point p = sinks[3];
point p1 = sinks[7];
///////
//  kt_rectangle range(p, p1);

cout << "sinks.size: " << sinks.size() << endl; //@ j

cout << "Creating kdtrees:" << endl;

kd_tree tree(sinks);

ctTopology *top = tree.create(); //(Direction dir = VERTICAL);///////

bt_node<point> root1 = top->get_root();
bt_node<point> *root = &root1;

/**  kd_tree tree(sinks1);
ctTopology* top = tree.create();
bt_node<point> root3 = top->get_root();
bt_node<point>* root = &root3;
*/
tree.report_subtree1(root);

cout << endl;
tree.select_point(top, p);
cout << tree._preset_size;

cout << endl;
tree.select_rect(top, p, p1);
cout << tree._preset_size;
cout << endl;
// cout<< "Searching kdtrees:" <<endl;
// tree.search_kd_tree(root, range);

// cout<< tree._preset_size;
// cout<<endl;

// cout<<"Searching again2:" <<endl;
// kt_rectangle region = tree.get_region(root);
// tree.search_kd_tree(root, range);
}

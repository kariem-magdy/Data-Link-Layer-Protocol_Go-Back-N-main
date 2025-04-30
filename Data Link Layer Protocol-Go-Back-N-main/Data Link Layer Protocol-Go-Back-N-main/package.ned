package cn_project;

@license(LGPL);
//
// TODO documentation
//
network Network
{
    parameters:
       	 int WS;
  		 double TO ;
         double PT ;
         double TD ;
         double ED ;
         double DD ;
         double LP ;
    @display("bgb=851,306");
    submodules:
        Node0: Node {
            @display("p=149,114");
        }
        Node1: Node {
            @display("p=397,225");
        }
        Coordinator: Coordinator {
            @display("p=561,84");
        }
    connections:
        Coordinator.out0 --> Node0.inC;
        Coordinator.out1 --> Node1.inC;
        Node0.out --> Node1.inN;
        Node1.out --> Node0.inN;
}

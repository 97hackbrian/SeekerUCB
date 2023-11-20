import rclpy
from rclpy.node import Node
from std_msgs.msg import String

class SubscriberNode(Node):
    def __init__(self):
        super().__init__('subscriber_node')
        self.subscription = self.create_subscription(
            String, 'topic', self.callback, 10)
        self.subscription  # para evitar que sea recolectado por el recolector de basura
        self.get_logger().info('Nodo suscriptor iniciado')

    def callback(self, msg):
        self.get_logger().info('Recibido: "%s"' % msg.data)

def main(args=None):
    rclpy.init(args=args)
    node = SubscriberNode()
    rclpy.spin(node)
    node.destroy_node()
    rclpy.shutdown()

if __name__ == '__main__':
    main()

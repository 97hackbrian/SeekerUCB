import rclpy
from rclpy.node import Node
from std_msgs.msg import String

class PublisherNode(Node):
    def __init__(self):
        super().__init__('publisher_node')
        self.publisher = self.create_publisher(String, 'topic', 10)
        timer_period = 1  # segundos
        self.timer = self.create_timer(timer_period, self.publish_message)
        self.get_logger().info('Nodo publicador iniciado')

    def publish_message(self):
        message = String()
        message.data = '¡Hola, ROS 2!'
        self.publisher.publish(message)
        self.get_logger().info('Publicando: "%s"' % message.data)

def main(args=None):
    rclpy.init(args=args)
    node = PublisherNode()
    rclpy.spin(node)
    node.destroy_node()
    rclpy.shutdown()

if __name__ == '__main__':
    main()

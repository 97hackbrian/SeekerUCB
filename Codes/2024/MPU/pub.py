import rclpy
from std_msgs.msg import String  # Importa el tipo de mensaje que deseas publicar

def main():
    rclpy.init()

    node = rclpy.create_node('mi_nodo_publicador')  # Cambia 'mi_nodo_publicador' al nombre que prefieras
    publisher = node.create_publisher(String, 'mi_topico', 10)  # Cambia 'mi_topico' al nombre que prefieras

    msg = String()  # Crea una instancia del mensaje que deseas publicar

    # Publica datos
    while rclpy.ok():
        msg.data = 'Hola, ROS 2!'
        node.get_logger().info('Publicando: "%s"' % msg.data)
        publisher.publish(msg)
        rclpy.spin_once(node)

    node.destroy_node()
    rclpy.shutdown()

if __name__ == '__main__':
    main()

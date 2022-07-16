#include <robot_dart/control/robot_control.hpp>
#include <robot_dart/robot_dart_simu.hpp>
#include <robot_dart/robots/arm.hpp>

#ifdef GRAPHIC
#include <robot_dart/gui/magnum/graphics.hpp>
#endif

// @ROBOT_CONTROL@
class MyController : robot_dart::control::RobotControl {
public:
    MyController(const Eigen::VectorXd& ctrl, bool full_control) : robot_dart::control::RobotControl(ctrl, full_control){}
    MyController(const Eigen::VectorXd& ctrl, const std::vector<std::string>& dof_names) : robot_dart::control::RobotControl(ctrl, dof_names){}
    
    void configure()
    {
        _active = true;
    }
    
    Eigen::VectorXd calculate(double)
    {
        auto robot = _robot.lock();
        auto cmd = 100 * (_ctrl - robot->positions(_controllable_dofs));
        return cmd;
    }
    std::shared_ptr<RobotControl> clone() const
    {
        return std::make_shared<RobotControl>(*this);
    }

private:
    bool active;
    bool full_control;
};

// @ROBOT_CONTROL_END@
int main()
{
    auto robot = std::make_shared<robot_dart::robots::Arm>();
    Eigen::VectorXd ctrl(4);
    ctrl << 0.0, 1.57, -0.5, 0.7;
    auto control = MyController(ctrl, false);
    robot->add_controller(control, 1);
    std::cout<< robot->positions() << std::endl;

    robot_dart::RobotDARTSimu simu(0.001); // dt=0.001, 1KHz simulation
    simu.add_floor();
    simu.add_robot(robot);
#ifdef GRAPHIC
    auto graphics = std::make_shared<robot_dart::gui::magnum::Graphics>();
    simu.set_graphics(graphics);
    graphics->look_at({0.5, 3., 0.75}, {0.5, 0., 0.2});
#endif

    simu.run(5.);
}
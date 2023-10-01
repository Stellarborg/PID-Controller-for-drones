from dronekit import connect, VehicleMode
from dronekit.mavlink import MAV_CMD_DO_SET_MODE
from pymavlink import mavutil
vehicle = connect('/dev/ttyUSB0', wait_ready=True)

waypoints = [
    (35.7749, -160.4194, 40),  # Waypoint 1
    (34.7749, -142.4194, 35),  # Waypoint 2
    (36.7749, -112.4194, 20),  # Waypoint 3
    (31.7749, -122.4194, 15)   # Waypoint 4
]

vehicle.mode = VehicleMode("GUIDED")
vehicle.armed = True
while not vehicle.armed:
    pass

print("Taking off drone to 40m above ground level")
vehicle.simple_takeoff(40)

while True:
    altitude = vehicle.location.global_relative_frame.alt
    if altitude >= 40 * 0.95:
        print("Reached target altitude")
        break

for waypoint in waypoints:
    latitude, longitude, altitude = waypoint
    target_location = vehicle.location.global_frame.get_distance_metres(latitude, longitude, altitude)

    print(f"Going to waypoint: ({latitude}, {longitude}, {altitude} meters AGL)")
    vehicle.simple_goto(target_location)
    vehicle.flush()

    while True:
        distance = vehicle.location.global_frame.get_distance_metres(latitude, longitude, altitude)
        if distance <= 1:
            print("Reached waypoint")
            break

print("Return to Launch")
vehicle.mode = VehicleMode("RTL")

vehicle.close()

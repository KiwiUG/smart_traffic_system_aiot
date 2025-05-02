import cv2
import time
import torch
import serial

# Configuration
PRIMARY_LANE = 1  # Main road
LANE_NAMES = {
    1: "Main (Primary)",
    2: "Right",
    3: "Opposite",
    4: "Left"
}

# Initialize model
model = torch.hub.load('ultralytics/yolov5', 'yolov5s')
model.conf = 0.3

# Initialize serial
try:
    ser = serial.Serial('/dev/cu.usbmodem11101', 9600, timeout=1)
    time.sleep(2)
except Exception as e:
    print(f"Serial error: {e}")
    exit()

# Initialize video feeds
videos = {
    1: "L_H_L_H.mov",  # Primary
    2: "H_L_H_L.mov",  # Right
    3: "L_H_L_H.mov",    # Opposite
    4: "H_L_H_L.mov"   # Left
}
caps = {lane: cv2.VideoCapture(path) for lane, path in videos.items()}

def get_car_count(frame):
    results = model(frame)
    return len(results.pred[0][results.pred[0][:, -1] == 2])

def send_lane_command(lane):
    try:
        ser.write(f"{lane}\n".encode())
        print(f"Sent command: {lane}")
    except Exception as e:
        print(f"Serial write error: {e}")
import time

def control_loop():
    try:
        while True:
            counts = {}
            
            # Initial counts to select the lane
            for lane, cap in caps.items():
                ret, frame = cap.read()
                if not ret:
                    cap.set(cv2.CAP_PROP_POS_FRAMES, 0)
                    ret, frame = cap.read()
                counts[lane] = get_car_count(frame)
            
            print("Current counts:", counts)
            
            # Find lane with max traffic (prioritize primary if tie)
            max_count = max(counts.values())
            if max_count == 0:
                send_lane_command(-1)  # All red
                time.sleep(1)
                continue
                
            candidates = [lane for lane, count in counts.items() if count == max_count]
            selected_lane = PRIMARY_LANE if PRIMARY_LANE in candidates else candidates[0]
            
            send_lane_command(selected_lane)
            
            green_time = min(max(max_count * 2, 5), 15)+3


            # Timer using real time
            start_time = time.time()
            elapsed_seconds = 0
            
            while elapsed_seconds < green_time:
                frames = {}
                for lane, cap in caps.items():
                    ret, frame = cap.read()
                    if not ret:
                        cap.set(cv2.CAP_PROP_POS_FRAMES, 0)
                        ret, frame = cap.read()
                    frames[lane] = frame
                
                remaining_time = green_time - elapsed_seconds
                for lane, frame in frames.items():
                    cv2.putText(frame, f"{LANE_NAMES[lane]}: {counts[lane]} cars", 
                                (20, 40), cv2.FONT_HERSHEY_SIMPLEX, 0.7, (0, 255, 0), 2)
                    cv2.putText(frame, f"Timer: {int(remaining_time)}s", (20, 80),
                                cv2.FONT_HERSHEY_SIMPLEX, 0.7, (0, 255, 255), 2)
                    cv2.imshow(f'Lane {lane}', frame)
                
                if cv2.waitKey(1) & 0xFF == ord('q'):
                    raise KeyboardInterrupt

                # update elapsed time
                elapsed_seconds = int(time.time() - start_time)
                
    finally:
        send_lane_command(-1)
        for cap in caps.values():
            cap.release()
        cv2.destroyAllWindows()
        ser.close()

if __name__ == "__main__":
    control_loop()
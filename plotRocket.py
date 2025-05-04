import pandas as pd
import matplotlib.pyplot as plt
import numpy as np
plt.figure(figsize=(10, 6))
circle1= plt.Circle((0, 0), 63780, color='b', fill=False)
circle2=plt.Circle((0, 0), 2500000, color='r', fill=False)
df = pd.read_csv('rocket_simulation.csv')
plt.plot(df['Y(m)'], df['X(m)'], label='X Position (m)')
plt.xlabel('Position (x)')  
plt.ylabel('Position (y)')
plt.title('Rocket Position Over Time')
plt.gca().add_patch(circle1)
plt.gca().add_patch(circle2)
plt.show()

# Start with a JDK base image
FROM openjdk:17-jdk-slim

# Set work directory
WORKDIR /app

# Copy compiled jar (will add Maven build later)
COPY target/motion-api-1.0.0.jar app.jar

# Expose port
EXPOSE 8080

# Set JVM lib path to access the C++ wrapper
ENV JAVA_OPTS="-Djava.library.path=/app/libs"

# Copy native library
COPY ./libs/libmotionwrapper.so  /app/libs/
# Run the app
ENTRYPOINT ["java", "-Djava.library.path=/app/libs", "-jar", "app.jar"]



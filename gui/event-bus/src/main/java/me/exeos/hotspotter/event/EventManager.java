package me.exeos.hotspotter.event;

import java.lang.reflect.InvocationTargetException;
import java.lang.reflect.Method;
import java.util.*;

public class EventManager {


    private final HashMap<Class<? extends Event>, HashMap<Object, List<Method>>> subscribers = new HashMap<>();

    public void subscribe(Object target) {
        for (Method method : target.getClass().getDeclaredMethods()) {
            if (method.isAnnotationPresent(EventHandler.class) && method.getParameterCount() == 1 && Event.class.isAssignableFrom(method.getParameterTypes()[0])) {
                method.setAccessible(true);

                @SuppressWarnings("unchecked")
                Class<? extends Event> eventType = (Class<? extends Event>) method.getParameterTypes()[0];
                subscribers
                        .computeIfAbsent(eventType, _ -> new HashMap<>())
                        .computeIfAbsent(target, _ -> new ArrayList<>())
                        .add(method);
            }
        }
    }

    public void unsubscribe(Object target) {
        for (HashMap<Object, List<Method>> map : subscribers.values()) {
            map.remove(target);
        }
    }

    public void fire(Event event) {
        HashMap<Object, List<Method>> subMap = subscribers.get(event.getClass());
        if (subMap == null) {
            return;
        }

        for (Map.Entry<Object, List<Method>> entry : subMap.entrySet()) {
            for (Method method : entry.getValue()) {
                try {
                    method.invoke(entry.getKey(), event);
                } catch (IllegalAccessException e) {
                    throw new RuntimeException(e);
                } catch (InvocationTargetException e) {
                    throw new RuntimeException(e);
                }
            }
        }
    }
}
